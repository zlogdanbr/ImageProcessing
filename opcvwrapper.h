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

bool loadImage(const std::string& image_path, Mat& img);
void showImage(const Mat& img, const std::string& title);
bool saveImage(const std::string& image_path, Mat& img);
Mat convertograyScale(const Mat& img);
Mat equalizeGrayImage(const Mat& img);
Mat equalizeColorImage(const Mat& img);
Mat flipImageHorizontal(const Mat& img);
Mat flipImageVertical(const Mat& img);
Mat flipImage(const Mat& img);
Mat SumImages(const Mat& img1, const Mat& img2);
Mat SubImages(const Mat& img1, const Mat& img2);
Mat XorImages(const Mat& img1, const Mat& img2);

Mat blurImageSmooth(const Mat& img, int kernel_size);
Mat GaussianImageSmooth(const Mat& img, int kernel_size);
Mat MedianImageSmooth(const Mat& img, int kernel_size);
Mat ApplyCustom2Dfilter(const Mat& img, Mat& kernel, int ddepth, Point& anchor, double delta);
Mat laplacian(Mat& src);
Mat convertRectoImg(Rect& r, Mat& img);
void drawCirclesAtImgFromRoi(Mat& img, Rect& roi);
void drawSquaresAtImgFromRoi(Mat& img, Rect& roi);
std::vector<Mat> splitChannel(Mat& img);
void segmentationOfROI(Mat& img, Rect& roi,int,int,int);
int findcontours(const Mat& img, RoiAretype& contours, std::vector<Vec4i>& hierarchy, int thresh, Mat& edges);
void drawCountour(RoiAretype& contours, Mat& img, std::vector<Vec4i>& hierarchy);

#endif

//--------------------------------------------------------------------------------------------------