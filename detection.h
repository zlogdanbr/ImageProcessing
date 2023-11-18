#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
using namespace cv;

static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);
Mat detectEyes(const Mat& image);
Mat detectCorners(const Mat& image);
Mat detect(const Mat& image);
Mat detect2(const Mat& image);
Mat custom_algo(const Mat& image);
