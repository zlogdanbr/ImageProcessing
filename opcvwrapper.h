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

const std::string CASCADE_PATH_FRONTAL_EXTENDED = "data\\haarcascades\\haarcascade_frontalcatface_extended.xml";
const std::string CASCADE_PATH_FRONTAL = "data\\haarcascades\\haarcascade_frontalface_default.xml";
const std::string CASCADE_PATH_FRONTAL_EYE = "data\\haarcascades\\haarcascade_eye.xml";
const std::string CASCADE_PATH_FRONTAL_EYE_CUDA = "data\\haarcascades_cuda\\haarcascade_eye.xml";
const std::string fd_modelPath = "face_data1\\face_detection_yunet_2023mar_int8.onnx";
const std::string fr_modelPath = "face_data2\\face_recognition_sface_2021dec.onnx";

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


/*
*  Helper functions
*/
static
void visualize(Mat& input, int frame, Mat& faces, int thickness = 2);
AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints);;
void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE, UBYTE, UBYTE, bool blank_bgr = false);
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

//--------------------------------------------------------------------------------------------------