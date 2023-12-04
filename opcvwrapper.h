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
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Eigen"
#include "Dense"
#include "filesys.h"


using namespace cv;

using UBYTE = unsigned char;
using AbstractRegion = std::vector<std::pair<int, int>>;
using TargetPoints = std::pair<std::vector<int>, std::vector<int>>;
using TargetPointsDouble = std::pair<std::vector<double>, std::vector<double>>;
using RoiAretype = std::vector< std::vector<Point> >;

/**
* basic operation
*/
bool loadImage(const std::string& image_path, Mat& img);
void showImage(const Mat& img, const std::string& title);
bool saveImage(const std::string& image_path, Mat& img);
Mat flipImageHorizontal(const Mat& img);
Mat flipImageVertical(const Mat& img);
Mat flipImage(const Mat& img);
Mat InvertImage(const Mat& img);
Mat getBinaryImage(const Mat& img);
Mat adjustContrast(const Mat& img, int factor);
Mat adjustBrightness(const Mat& img, int factor);


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
Mat ApplyHoughTransformCustom(const Mat& img);

/* 
* Edge detectors Laplacian
*/
Mat ApplyLaplacian(const Mat& src);
Mat ApplyLaplacianExtended( const Mat& src, 
                            int kernel_size = 3,
                            int scale = 1, 
                            int delta = 0, 
                            int ddepth = CV_16S);


/*
* Edge detectors Sobel
*/
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

/*
* Edge detectors Sharpening/Unsharp
*/
Mat Sharpening(const Mat& img);
Mat Unsharp(const Mat& img);

/*
*  Morphological
*/
Mat ApplyErode(const Mat& img);
Mat ApplyDilate(const Mat& img);
Mat ApplyClosing(const Mat& img);
Mat ApplyOpening(const Mat& img);
Mat ApplyMorphGradient(const Mat& img);
Mat ApplyTopHatAlgo(const Mat& img);


/*
*  Helper functions
*/
static
void visualize(Mat& input, int frame, Mat& faces, int thickness = 2);

AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);

void highlightFeature(  Mat& img, 
                        AbstractRegion& abstract_region, 
                        UBYTE, 
                        UBYTE, 
                        UBYTE,
                        bool blank_bgr = false);

TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints);

Mat convertRectoImg(Rect& r, Mat& img);

std::vector<Mat> splitChannel(Mat& img);

std::vector< Eigen::MatrixXd > convertOpenCVToEigen(Mat& OpenCVImage);

void drawCirclesAtImgFromRoi(Mat& img, Rect& roi);
void drawSquaresAtImgFromRoi(Mat& img, Rect& roi);


/*
*  Advanced Algorithms and detectors
*/
Mat detectCornersHarrisAlgoFull(const Mat& image,
                                int neighborhood_size,
                                int aperture_size,
                                double threshold,
                                double Harris_parameter
                                );

Mat detectCornersHarris(const Mat& image);
Mat detectFastKeyPoints(const Mat& image);
Mat workingAlgorithm(const Mat& image);
Mat ApplyCanny(const Mat& img);
Mat ApplyCannyAlgoFull(const Mat& img, int threshold = 125, int aperture = 350);
Mat WaterShed(const Mat& img);



#endif

//--------------------------------------------------------------------------------------------------