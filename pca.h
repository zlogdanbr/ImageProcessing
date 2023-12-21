#pragma once
#include "opcvwrapper.h"

// Source
// https://docs.opencv.org/4.x/d1/dee/tutorial_introduction_to_pca.html

void drawAxis(Mat&, Point, Point, Scalar, const float);
double getOrientation(const std::vector<Point>&, Mat&);
Mat ApplyBasicSegmentation(const Mat& img, int opt = 0);
Mat ApplyPCA(const Mat& img);
Mat ApplyFindContourns(const Mat& img);