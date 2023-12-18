//--------------------------------------------------------------------------------------------------
// These functions have been taken from OpenCV documentation and made easier to use
// author: Daniel Vasconcelos Gomes 2022/2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
#ifndef _CVWRAPPER_
#define _CVWRAPPER_

#include "image_util.h"
#include <thread>

/**
* basic operations
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
Mat adjustGama(const Mat& img, double);

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
* Filters
*/
Mat ApplyThreShold(const Mat& img, double _threshold);
Mat blurImageSmooth(const Mat& img, int kernel_size);
Mat ApplyBilateralFilterExt(    const Mat& img, 
                                int kernel_size, 
                                double sigma1, 
                                double sigma2);

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
Mat ApplyHoughTransformCustom(const Mat& img);

/* 
* Edge detectors Laplacian
*/
Mat ApplyLaplacianExtended( const Mat& src, 
                            int kernel_size = 3,
                            int scale = 1, 
                            int delta = 0, 
                            int ddepth = CV_16S);


/*
* Edge detectors Sobel
*/
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
Mat ApplyErodeEx(const Mat& img,int);
Mat ApplyDilateEx(const Mat& img,int);

Mat ApplyClosing(const Mat& img);
Mat ApplyOpening(const Mat& img);
Mat ApplyMorphGradient(const Mat& img);
Mat ApplyTopHatAlgo(const Mat& img);

/*
*  Advanced Algorithms and detectors
*/
Mat detectCornersHarrisAlgoFull(const Mat& image,
                                int neighborhood_size,
                                int aperture_size,
                                double threshold,
                                double Harris_parameter
                                );


Mat ApplyCustomAlgo(const Mat& image);
Mat ApplyCannyAlgoFull(const Mat& img, int threshold = 125, int aperture = 350);
Mat ApplyDoG(const Mat& im);

// https://learnopencv.com/deep-learning-with-opencvs-dnn-module-a-definitive-guide/
Mat NN(const Mat& img);

Mat segmentationDVG(Mat& image);

#endif

//--------------------------------------------------------------------------------------------------