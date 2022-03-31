cmake_minimum_required(VERSION 3.21)
project(DFTV2)
find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
add_executable(DFTV2 main.cpp MY_DFT.cpp MY_DFT.h)
target_include_directories(DFTV2 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_directories(DFTV2 PRIVATE)
target_link_libraries(DFTV2 PRIVATE ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})