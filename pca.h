#pragma once
#include "opcvwrapper.h"
#include <iostream>
#include <string>
#include <sstream>

constexpr int _dim2D = 2;

using eigenvector = std::vector<Point2d>;
using eigenvalue = std::vector<double>;
using center = std::pair<double, double>;
using eigenvectors = std::vector< eigenvector >;
using eigenvalues = std::vector< eigenvalue  >;
using centers = std::vector<center>;
using eigenSpace = std::pair<eigenvectors, eigenvalues>;
using contourns = std::vector<std::vector<Point> >;
using PointValue = std::pair<std::vector<Point2d>, std::vector<double>>;
using Distances = std::vector<double>;
using Angles = std::vector<double>;

std::vector<std::vector<Point> > getContourns(const Mat& img);
eigenSpace getEingenSpace(const contourns& contours, Mat& src, centers& _centers);
PointValue getEingenFromContourn(const std::vector<Point>& pts, Mat& img, center& c);
std::stringstream getEingenSpaceInfo(const Mat& img);
PCA getPCAAnalysis(const std::vector<Point>& pts);

Mat Rebuild(PCA& pca, Mat& compressed, int channels, int rows);
Mat TestPCA(const Mat& img);

double calculateDistance2D(eigenvector& e);
double Angle2D(eigenvector& e);
int dotProduct2D(Point2d& v1, Point2d& v2);
double calculateNorm2D(Point2d& v1);


