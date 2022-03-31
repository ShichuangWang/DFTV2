
#include "MY_DFT.h"


void My_DFT(Mat input_image, Mat& output_image, Mat& transform_image)
{
    int m = getOptimalDFTSize(input_image.rows);
    int n = getOptimalDFTSize(input_image.cols);
    copyMakeBorder(input_image, input_image, 0, m - input_image.rows, 0, n - input_image.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(input_image), Mat::zeros(input_image.size(), CV_32F) };

    merge(planes, 2, transform_image);

    dft(transform_image, transform_image);

    split(transform_image, planes);
    Mat transform_image_real = planes[0];
    Mat transform_image_imag = planes[1];

    magnitude(planes[0], planes[1], output_image);

    output_image += Scalar(1);
    log(output_image, output_image);
    normalize(output_image, output_image, 0, 1, NORM_MINMAX);


    output_image = output_image(Rect(0, 0, output_image.cols & -2, output_image.rows & -2));

    int cx = output_image.cols / 2;
    int cy = output_image.rows / 2;
    Mat q0(output_image, Rect(0, 0, cx, cy));
    Mat q1(output_image, Rect(cx, 0, cx, cy));
    Mat q2(output_image, Rect(0, cy, cx, cy));
    Mat q3(output_image, Rect(cx, cy, cx, cy));


    Mat tmp;
    q0.copyTo(tmp); q3.copyTo(q0); tmp.copyTo(q3);
    q1.copyTo(tmp); q2.copyTo(q1); tmp.copyTo(q2);


    Mat q00(transform_image_real, Rect(0, 0, cx, cy));
    Mat q01(transform_image_real, Rect(cx, 0, cx, cy));
    Mat q02(transform_image_real, Rect(0, cy, cx, cy));
    Mat q03(transform_image_real, Rect(cx, cy, cx, cy));
    q00.copyTo(tmp); q03.copyTo(q00); tmp.copyTo(q03);
    q01.copyTo(tmp); q02.copyTo(q01); tmp.copyTo(q02);

    Mat q10(transform_image_imag, Rect(0, 0, cx, cy));
    Mat q11(transform_image_imag, Rect(cx, 0, cx, cy));
    Mat q12(transform_image_imag, Rect(0, cy, cx, cy));
    Mat q13(transform_image_imag, Rect(cx, cy, cx, cy));
    q10.copyTo(tmp); q13.copyTo(q10); tmp.copyTo(q13);
    q11.copyTo(tmp); q12.copyTo(q11); tmp.copyTo(q12);

    planes[0] = transform_image_real;
    planes[1] = transform_image_imag;
    merge(planes, 2, transform_image);
}
