#ifndef _DETECT_ALGO_FILE_
#define _DETECT_ALGO_FILE_

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "opcvwrapper.h"
using namespace cv;

using UBYTE = unsigned char;
using AbstractRegion = std::vector<std::pair<int, int>>;

static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);

Mat detectEyes(const Mat& image);
Mat detectCorners(const Mat& image);
Mat detect(const Mat& image);
Mat custom_algo(const Mat& image);
Mat convertRectoImg(Rect& r, Mat& img);
std::vector<Mat> splitChannel(Mat& img);


Mat grabRegion(const Mat& img, cv::Rect& rectangle);

AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);

void highlightFeature(Mat& img, Rect& roi, UBYTE, UBYTE, UBYTE);
void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE, UBYTE, UBYTE);

#endif
