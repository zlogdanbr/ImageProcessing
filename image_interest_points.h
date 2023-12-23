#pragma once

#include "opcvwrapper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits.h>

constexpr int RECTANGLE_APROX = 1;
constexpr int CIRCLE_APROX = 2;
constexpr int POLYGONAL_APROX = 3;
constexpr int CONVEXHULL_APROX = 4;



struct ImageComponentsDescriptor
{
	std::vector<cv::Point> region;
	std::vector<cv::Point> regionHull;
	std::vector<cv::Point> regionAprox;

	cv::Moments momInertia;
	cv::Moments momInertiaHull;
	cv::Moments momInertiaAprox;

	bool convex = false;
	bool convexHull = false;
	bool convexAprox = false;
};

using ObjectsCollection = std::vector< ImageComponentsDescriptor>;
using RegionPoints		= std::vector<std::vector<Point> >;

RegionPoints 
	detectRegions(const Mat& img);

ObjectsCollection 
	getObjectsInfo(RegionPoints& raw_contourns);

std::vector<cv::Point> 
	getMomentOfRegionCollection(ImageComponentsDescriptor& inf);

std::stringstream  
	getImageInfoMoments(const Mat& img);

std::pair<int, int> 
	getCentroid(cv::Moments& momInertia);

double 
	getArea(std::vector<cv::Point>& region);

bool 
	invalid(	std::pair<int, int>& centroid, 
				double& area, 
				double area_threshold_min = 0, 
				double area_threshold_max = 2000);

