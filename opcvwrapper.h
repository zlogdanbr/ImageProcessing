//--------------------------------------------------------------------------------------------------
// These functions have been taken from OpenCV documentation and made easier to use
// author: Daniel Vasconcelos Gomes 2022
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
#ifndef _CVWRAPPER_
#define _CVWRAPPER_

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
#include "Eigen"
#include "Dense"


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
Mat getBinaryImage(const Mat& img);
std::vector<Mat> splitChannel(Mat& img);
std::vector< Eigen::MatrixXd > convertOpenCVToEigen(Mat& OpenCVImage);
void drawCirclesAtImgFromRoi(Mat& img, Rect& roi);
void drawSquaresAtImgFromRoi(Mat& img, Rect& roi);

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
Mat GaussianImageSmoothExtended(    const Mat& img,
                                    int kernel_size,
                                    double sigmaX,
                                    double sigmaY
                                );
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
Mat ApplyLaplacianExtended(const Mat& src, int kernel_size = 3, int scale = 1, int delta = 0, int ddepth = CV_16S);


Mat ApplySobelX(const Mat& img, int kernel_size);
Mat ApplySobelY(const Mat& img, int kernel_size);
Mat ApplySobel(const Mat& img, int kernel_size);

Mat ApplySobelXExtended(const Mat& img,
    int image_type,
    int depth,
    int type,
    double delta,
    int kernel_size);

Mat ApplySobelYExtended(const Mat& img,
    int image_type,
    int depth,
    int type,
    double delta,
    int kernel_size);

Mat ApplySobelExtended(const Mat& img,
    int image_type,
    int depth,
    int type,
    double delta,
    int kernel_size);

Mat Sharpening(const Mat& img);
Mat Unsharp(const Mat& img);

/*
*  Morphological
*/

Mat ApplyErode(const Mat& img);
Mat ApplyDilate(const Mat& img);
Mat ApplyMorphGradient(const Mat& img);
Mat ApplyTopHatAlgo(const Mat& img);

#endif

//--------------------------------------------------------------------------------------------------