#include<iostream>
#include<opencv2/opencv.hpp>
#include "MY_DFT.h"

using namespace cv;
using namespace std;

void high_pass_filter(int &origin_x,int &origin_y,int &Radius,Mat &DFT_Output,Mat &spectrogram,Mat &iDFT_Output){
    Mat planes[] = { Mat_<float>(DFT_Output), Mat::zeros(DFT_Output.size(),CV_32F) };
    split(spectrogram, planes);
    Mat spectrogram_real = planes[0];
    Mat spectrogram_imag = planes[1];
    for (int i = 0; i < spectrogram.rows; i++)
    {
        for (int j = 0; j < spectrogram.cols; j++)
        {
            if (((i - origin_x) * (i - origin_x) + (j - origin_y) * (j - origin_y)) < Radius * Radius)
            {
                spectrogram_real.at<float>(i, j) = 0;
                spectrogram_imag.at<float>(i, j) = 0;
            }
        }
    }
    planes[0] = spectrogram_real;
    planes[1] = spectrogram_imag;
    Mat image_transform_ilpf;
    merge(planes, 2, image_transform_ilpf);

    Mat iDft[] = { Mat_<float>(DFT_Output), Mat::zeros(DFT_Output.size(),CV_32F) };
    idft(image_transform_ilpf, image_transform_ilpf);
    split(image_transform_ilpf, iDft);
    magnitude(iDft[0], iDft[1], iDft[0]);
    normalize(iDft[0], iDft[0], 0, 1, NORM_MINMAX);
    iDFT_Output=iDft[0].clone();
}

int main()
{
    Mat image, image_gray, image_output, image_transform;
    image = imread("Rem.jpeg");
    if (image.empty())
    {
        cout << "There is no image!" << endl;
        return -1;
    }
    namedWindow("image",0);
    imshow("image", image);

    cvtColor(image, image_gray, COLOR_BGR2GRAY);
    namedWindow("image_gray",0);
    imshow("image_gray", image_gray);

    My_DFT(image_gray, image_output, image_transform);
    namedWindow("image_output",0);
    imshow("image_output", image_output);

//    Mat planes[] = { Mat_<float>(image_output), Mat::zeros(image_output.size(),CV_32F) };
//    split(image_transform, planes);
//    Mat image_transform_real = planes[0];
//    Mat image_transform_imag = planes[1];
//
    int core_x = image_transform.rows / 2;
    int core_y = image_transform.cols / 2;
    int r = 150;
//    for (int i = 0; i < image_transform_real.rows; i++)
//    {
//        for (int j = 0; j < image_transform_real.cols; j++)
//        {
//            if (((i - core_x) * (i - core_x) + (j - core_y) * (j - core_y)) < r * r)
//            {
//                image_transform_real.at<float>(i, j) = 0;
//                image_transform_imag.at<float>(i, j) = 0;
//            }
//        }
//    }
//    planes[0] = image_transform_real;
//    planes[1] = image_transform_imag;
//    Mat image_transform_ilpf;
//    merge(planes, 2, image_transform_ilpf);
//
//    Mat iDft[] = { Mat_<float>(image_output), Mat::zeros(image_output.size(),CV_32F) };
//    idft(image_transform_ilpf, image_transform_ilpf);
//    split(image_transform_ilpf, iDft);
//    magnitude(iDft[0], iDft[1], iDft[0]);
//    normalize(iDft[0], iDft[0], 0, 1, NORM_MINMAX);
//    namedWindow("idft",0);
//    imshow("idft", iDft[0]);
    Mat idft;
    high_pass_filter(core_x,core_y,r,image_output,image_transform,idft);
    namedWindow("idft",0);
    imshow("idft",idft);

    waitKey(0);
    return 0;
}
