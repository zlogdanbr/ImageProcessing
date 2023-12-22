#pragma once

#include "pca.h"
#include <limits.h>

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
std::vector<cv::Point> getMomentOfRegionCollection(InfoRegions& inf);
std::stringstream getImageInfoMoments(const Mat& img);
std::pair<int, int> getCentroid(cv::Moments& momInertia);

bool invalid(std::pair<int, int>& centroid, double& area);

