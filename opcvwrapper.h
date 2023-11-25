#ifndef _CVWRAPPER_
#define _CVWRAPPER_

//--------------------------------------------------------------------------------------------------
// These functions have been taken from OpenCV documentation and made easier to use
// author: Daniel Vasconcelos Gomes 2022
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#include "opencv2/objdetect.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/core_c.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>


using namespace cv;


using RoiAretype = std::vector< std::vector<Point> >;


/**
* Helper functions
*/
bool loadImage(const std::string& image_path, Mat& img);
void showImage(const Mat& img, const std::string& title);
bool saveImage(const std::string& image_path, Mat& img);
Mat flipImageHorizontal(const Mat& img);
Mat flipImageVertical(const Mat& img);
Mat flipImage(const Mat& img);
Mat InvertImage(const Mat& img);

/*
* Gray Scale
*/
bool isGrayScaleImage(const Mat& img);
Mat convertograyScale(const Mat& img);

/*
* Equalization
*/
Mat equalizeGrayImage(const Mat& img);
Mat equalizeColorImage(const Mat& img);


/*
* Fiters
*/
Mat ApplyThreShold(const Mat& img);
Mat blurImageSmooth(const Mat& img, int kernel_size);
Mat GaussianImageSmooth(const Mat& img, int kernel_size);
Mat MedianImageSmooth(const Mat& img, int kernel_size);
Mat ApplyCustomKernel(const Mat& img, Mat& kernel);

/*
* Hough Transform
*/
Mat ApplyHoughTransform(const Mat& img, int opt);
Mat ApplyHoughTransformRegular(const Mat& img);
Mat ApplyHoughTransformReProbabilistic(const Mat& img);

/* 
* Edge detectors
*/
Mat ApplyLaplacian(const Mat& src);
Mat ApplySobelX(const Mat& img, int kernel_size);
Mat ApplySobelY(const Mat& img, int kernel_size);
Mat ApplySobel(const Mat& img, int kernel_size);
Mat Sharpening(const Mat& img);
Mat Unsharp(const Mat& img);



#endif

//--------------------------------------------------------------------------------------------------