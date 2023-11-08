#ifndef _CVWRAPPER_
#define _CVWRAPPER_

#include "opencv2/objdetect.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

const std::string CASCADE_PATH_FRONTAL_EXTENDED	= "C:\\Users\\Administrador\\lib\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalcatface_extended.xml";
const std::string CASCADE_PATH_FRONTAL			= "C:\\Users\\Administrador\\lib\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
const std::string CASCADE_PATH_FRONTAL_EYE		= "C:\\Users\\Administrador\\lib\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
const std::string CASCADE_PATH_FRONTAL_EYE_CUDA	= "C:\\Users\\Administrador\\lib\\opencv\\sources\\data\\haarcascades_cuda\\haarcascade_eye.xml";


using namespace cv;

typedef std::vector< std::vector<Point> > RoiAretype;

bool loadImage(const std::string& image_path, Mat& img);
void showImage(const Mat& img, const std::string& title);
bool saveImage(const std::string& image_path, Mat& img);
Mat convertograyScale(const Mat& img);
Mat equalizeGrayImage(const Mat& img);
Mat equalizeColorImage(const Mat& img);
Mat blurImageSmooth(const Mat& img, int kernel_size);
Mat GaussianImageSmooth(const Mat& img, int kernel_size);
Mat MedianImageSmooth(const Mat& img, int kernel_size);
Mat ApplyCustom2Dfilter(const Mat& img, Mat& kernel, int ddepth, Point& anchor, double delta);
Mat laplacian(Mat& src);
Mat convertRectoImg(Rect& r, Mat& img);
std::vector<Rect> detectEyesInImage(Mat& img);
void drawCirclesAtImgFromRoi(Mat& img, Rect& roi);
void drawSquaresAtImgFromRoi(Mat& img, Rect& roi);
std::vector<Mat> splitChannel(Mat& img);
void segmentationOfROI(Mat& img, Rect& roi,int,int,int);
int findcontours(const Mat& img, RoiAretype& contours, std::vector<Vec4i>& hierarchy, int thresh, Mat& edges);
void drawCountour(RoiAretype& contours, Mat& img, std::vector<Vec4i>& hierarchy);

#endif