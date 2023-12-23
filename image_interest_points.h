#pragma once

#include "opcvwrapper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <limits.h>


struct ComponentsDescriptor
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

using ObjectsCollection = std::vector< ComponentsDescriptor>;
using RegionPoints = std::vector<std::vector<Point> >;

class CImageComponentsDescriptor final
{
public:

	CImageComponentsDescriptor(const Mat& img) :original_image(img) {};
	~CImageComponentsDescriptor() { original_image.deallocate(); };
	void detectRegions();
	void getObjectsInfo();
	ObjectsCollection getImageFullInformation()const { return Objects; };
	std::pair<int, int> getCentroid(cv::Moments& momInertia) const;
	double getArea(std::vector<cv::Point>& region) const;

private:

	CImageComponentsDescriptor(CImageComponentsDescriptor&) = delete;
	CImageComponentsDescriptor& operator=(CImageComponentsDescriptor&) = delete;

	ComponentsDescriptor ImageComponentsDescriptor;
	ObjectsCollection Objects;
	RegionPoints raw_contourns;
	Mat original_image;

	

	bool invalid(	std::pair<int, int>& centroid,
					double& area,
					double area_threshold_min = 0,
					double area_threshold_max = 2000) const;

};

std::stringstream getImageInfoMoments(const Mat& img);


