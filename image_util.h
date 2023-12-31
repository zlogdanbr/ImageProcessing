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
#include "wx/wx.h"
#include <wx/gdicmn.h> 
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "filesys.h"
#include <thread>
#define CVPLOT_HEADER_ONLY 
#include <CvPlot/cvplot.h>

using namespace cv;
using namespace dnn;

namespace image_util
{
	using UBYTE = unsigned char;
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
	using FunctionHarris          = std::function<Mat(Mat, int, int, double, double)>;

	using Function1ParContainer = std::map < wxString, Function1Parameter >;
	using Function2ParContainer = std::map < wxString, Function2Parameter >;
	using Function3ParContainer = std::map < wxString, Function3Parameters >;
	using Function4ParContainer = std::map < wxString, Function4Parameters >;
	using Function5ParContainer = std::map < wxString, Function5Parameters >;
	using Function2SliderContainer = std::map < wxString, Function2Slider >;
	using FunctionSobelParametersContainer = std::map < wxString, FunctionSobelParameters >;
	using FunctionHarrisParametersContainer = std::map < wxString, FunctionHarris >;

	using CPointCst = std::pair<int, int>;
	using CDataValue = std::vector<CPointCst>;
	using RGB_CST = unsigned char[3];

	wxImage wx_from_mat(Mat& img);
	Mat mat_from_wx(wxImage& wx);

	Mat fitImageOnScreen(Mat& img, int wscreen, int hscreen);

	void showManyImagesOnScreen(std::vector<Mat>& images);

	std::pair< std::vector<int>, std::vector<int>>
	getImageXY(std::vector<std::vector<Point> >& raw_contourns);

	void drawCountourXY(std::vector<std::vector<Point> >& raw_contourns);



}
