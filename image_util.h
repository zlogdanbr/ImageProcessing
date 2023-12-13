#pragma once

#include "opencv2/objdetect.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "filesys.h"
#include "wx/wx.h"
#include <wx/gdicmn.h> 

using namespace cv;

namespace image_util
{
	using UBYTE = unsigned char;
	using AbstractRegion = std::vector<std::pair<int, int>>;
	using TargetPoints = std::pair<std::vector<int>, std::vector<int>>;
	using TargetPointsDouble = std::pair<std::vector<double>, std::vector<double>>;
	using RoiAretype = std::vector< std::vector<Point> >;
	using RGB = unsigned char[3];

	AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);

	void highlightFeature(Mat& img,
		AbstractRegion& abstract_region,
		UBYTE,
		UBYTE,
		UBYTE,
		bool blank_bgr = false);

	TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints);

	Mat convertRectoImg(Rect& r, Mat& img);

	std::vector<Mat> splitChannel(Mat& img);

	void drawCirclesAtImgFromRoi(Mat& img, Rect& roi);
	void drawSquaresAtImgFromRoi(Mat& img, Rect& roi);

	wxImage wx_from_mat(Mat& img);
	Mat mat_from_wx(wxImage& wx);

	void putpixel(int i, int j, wxImage img, RGB& rgb);

	//------------------------------------------------------------------------
	// Mid Point Algorithm for circles
	// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	// Adapted from 
	// https://www.thecrazyprogrammer.com/2016/12/bresenhams-midpoint-circle-algorithm-c-c.html
	//------------------------------------------------------------------------
	void drawcircle(int x0, int y0, int radius, wxImage img, RGB& r);
}
