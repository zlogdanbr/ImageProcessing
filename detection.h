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
Mat detectCornersHarris(const Mat& image);
Mat fastDetectKeyPoints(const Mat& image);
Mat workingAlgorithm(const Mat& image);
Mat convertRectoImg(Rect& r, Mat& img);
std::vector<Mat> splitChannel(Mat& img);

AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);;
void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE, UBYTE, UBYTE, bool blank_bgr = false);

#endif
