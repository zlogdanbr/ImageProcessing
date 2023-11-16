#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
using namespace cv;

// It should work but only God knows why it is not
// I had used these functions before using Windows 10 and Linux ( Lubuntu 18 )
// I suspect that as I am running at windows 11, opencv simply does not like
// it when it runs detector->detect(out, faces1);

static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);
Mat detectEyes(const Mat& image);
