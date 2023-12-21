#pragma once
#include "opcvwrapper.h"

// Source
// https://docs.opencv.org/4.x/d1/dee/tutorial_introduction_to_pca.html

constexpr int _dim2D = 2;
using eigenvector = std::vector<Point2d>;
using eigenvalue = std::vector<double>;
using center = std::pair<double, double>;

using eigenvectors = std::vector< eigenvector >;
using eigenvalues = std::vector< eigenvalue  >;
using centers = std::vector<center>;
using eigenSpace = std::pair<eigenvectors, eigenvalues>;
using contourns = const std::vector<std::vector<Point> >;

void drawAxis(Mat&, Point, Point, Scalar, const float);
double getOrientation(const std::vector<Point>&, Mat&);
Mat ApplyBasicSegmentation(const Mat& img, int opt = 0);
Mat ApplyPCA(const Mat& img);
Mat ApplyFindContourns(const Mat& img);


eigenSpace getEingenSpace(const contourns& contours, centers& c);


