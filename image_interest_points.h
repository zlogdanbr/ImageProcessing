#pragma once

#include "pca.h"

constexpr int RECTANGLE_APROX = 1;
constexpr int CIRCLE_APROX = 2;
constexpr int POLYGONAL_APROX = 3;
constexpr int CONVEXHULL_APROX = 4;

struct InfoRegions
{
	std::vector<cv::Point> region;
	cv::Moments momInertia;
};

using ObjectsCollection = std::vector< InfoRegions>;

std::vector<std::vector<Point> > detectRegions(const Mat& img);

ObjectsCollection getObjectsInfo(std::vector<std::vector<Point> >& raw_contourns);

cv::Moments getMomentOfObjectsCollection(InfoRegions& inf);
std::vector<cv::Point> getMomentOfRegionCollection(InfoRegions& inf);

