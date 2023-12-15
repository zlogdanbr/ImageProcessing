//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#pragma once

#include "opencv2/objdetect.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "filesys.h"
#include "wx/wx.h"
#include <wx/gdicmn.h> 

using namespace cv;
using namespace dnn;

namespace image_util
{
	using UBYTE = unsigned char;
	using AbstractRegion = std::vector<std::pair<int, int>>;
	using TargetPoints = std::pair<std::vector<int>, std::vector<int>>;
	using TargetPointsDouble = std::pair<std::vector<double>, std::vector<double>>;
	using RoiAretype = std::vector< std::vector<Point> >;
	using RGB = unsigned char[3];

	using Function1Parameter = std::function<Mat(Mat)>;
	using Function2Parameter = std::function<Mat(Mat, int)>;
	using Function3Parameters = std::function<Mat(Mat, int, int)>;
	using Function4Parameters = std::function<Mat(Mat, int, double, double)>;
	using Function5Parameters = std::function<Mat(Mat, int, int, int, int)>;
	using Function2Slider = std::function<Mat(Mat, double)>;
	using FunctionSobelParameters = std::function<Mat(Mat, int, int, int, double, int)>;


	using Function1ParContainer = std::map < wxString, Function1Parameter >;
	using Function2ParContainer = std::map < wxString, Function2Parameter >;
	using Function3ParContainer = std::map < wxString, Function3Parameters >;
	using Function4ParContainer = std::map < wxString, Function4Parameters >;
	using Function5ParContainer = std::map < wxString, Function5Parameters >;
	using Function2SliderContainer = std::map < wxString, Function2Slider >;
	using FunctionSobelParametersContainer = std::map < wxString, FunctionSobelParameters >;

	using CPointCst = std::pair<int, int>;
	using CDataValue = std::vector<CPointCst>;
	using RGB_CST = unsigned char[3];

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

	Mat fitImageOnScreen(Mat& img, int wscreen, int hscreen);

	template<typename T1, typename T2>
	auto Plus(T1&& t1, T2&& t2) -> decltype(std::forward<T1>(t1) + std::forward<T2>(t2))
	{
		return std::forward<T1>(t1) + std::forward<T2>(t2);
	}

	template<typename T1, typename T2>
	auto Sub(T1&& t1, T2&& t2) -> decltype(std::forward<T1>(t1) - std::forward<T2>(t2))
	{
		return std::forward<T1>(t1) - std::forward<T2>(t2);
	}

	template<typename T1, typename T2, typename F>
	auto GenericFunction(T1&& t1, T2&& t2, F&& f) -> decltype(std::forward<T2>(t2))
	{
		return F(t1);
	}
}
