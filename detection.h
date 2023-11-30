//--------------------------------------------------------------------------------------------------
// Interface over OpenCV more complex algorithms
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
#ifndef _DETECT_ALGO_FILE_
#define _DETECT_ALGO_FILE_

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "opcvwrapper.h"
using namespace cv;

const std::string CASCADE_PATH_FRONTAL_EXTENDED = "data\\haarcascades\\haarcascade_frontalcatface_extended.xml";
const std::string CASCADE_PATH_FRONTAL = "data\\haarcascades\\haarcascade_frontalface_default.xml";
const std::string CASCADE_PATH_FRONTAL_EYE = "data\\haarcascades\\haarcascade_eye.xml";
const std::string CASCADE_PATH_FRONTAL_EYE_CUDA = "data\\haarcascades_cuda\\haarcascade_eye.xml";
const std::string fd_modelPath = "face_data1\\face_detection_yunet_2023mar_int8.onnx";
const std::string fr_modelPath = "face_data2\\face_recognition_sface_2021dec.onnx";

using UBYTE					= unsigned char;
using AbstractRegion		= std::vector<std::pair<int, int>>;
using TargetPoints			= std::pair<std::vector<int>, std::vector<int>>;
using TargetPointsDouble	= std::pair<std::vector<double>, std::vector<double>>;

/*
*  Helper functions
*/
static
void visualize(Mat& input, int frame, Mat& faces,  int thickness = 2);
AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);;
void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE, UBYTE, UBYTE, bool blank_bgr = false);
TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints);
Mat convertRectoImg(Rect& r, Mat& img);


/*
*  Advanced Algorithms and detectors
*/
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
Mat WaterShed(const Mat& img);


/*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*  These algorithms are not working on Windows 11 ( OpenCV 4.5 or 4.8 )
*  Maybe it is my graphic board or Windows 11
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
Mat detectFaces(const Mat& image);
std::vector<Rect> detectFacesInImage(Mat& img);
std::vector<Rect> detectEyesInImage(Mat& img);
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif

