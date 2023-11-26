#ifndef _DETECT_ALGO_FILE_
#define _DETECT_ALGO_FILE_

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "opcvwrapper.h"
using namespace cv;

using UBYTE					= unsigned char;
using AbstractRegion		= std::vector<std::pair<int, int>>;
using TargetPoints			= std::pair<std::vector<int>, std::vector<int>>;
using TargetPointsDouble	= std::pair<std::vector<double>, std::vector<double>>;

/*
*  Helper functions
*/
static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);
std::vector<Mat> splitChannel(Mat& img);
AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);;
void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE, UBYTE, UBYTE, bool blank_bgr = false);
TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints);
Mat convertRectoImg(Rect& r, Mat& img);

/* 
*  Advanced Algorithms and detectors
*/
Mat detectFaces(const Mat& image);

Mat detectCornersHarrisAlgoFull(	const Mat& image,
									int neighborhood_size,
									int aperture_size,
									int threshold,
									double Harris_parameter
									);


Mat detectCornersHarris(const Mat& image);
Mat detectFastKeyPoints(const Mat& image);
Mat workingAlgorithm(const Mat& image);

Mat ApplyCanny(const Mat& img);
Mat ApplyCannyAlgoFull(const Mat& img, int threshold = 125, int aperture = 350);

#endif

