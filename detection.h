#ifndef _DETECT_ALGO_FILE_
#define _DETECT_ALGO_FILE_

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "opcvwrapper.h"
using namespace cv;

static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);

Mat detectEyes(const Mat& image);
Mat detectCorners(const Mat& image);
Mat detect(const Mat& image);
Mat custom_algo(const Mat& image);
Mat convertRectoImg(Rect& r, Mat& img);
Mat findcontours(const Mat& img, RoiAretype& contours, std::vector<Vec4i>& hierarchy, int thresh);
std::vector<Mat> splitChannel(Mat& img);
void segmentationOfROI(Mat& img, Rect& roi, int, int, int);
void drawCountour(RoiAretype& contours, Mat& img, std::vector<Vec4i>& hierarchy);

#endif
