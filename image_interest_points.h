#pragma once

#include "opcvwrapper.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <limits.h>


struct ComponentsDescriptor
{
	std::vector<cv::Point> region;
	cv::Moments momInertia;
	bool convex = false;

	ComponentsDescriptor() {};

	ComponentsDescriptor(const ComponentsDescriptor& rhs)
	{
		region = rhs.region;
		momInertia = rhs.momInertia;
		convex = rhs.convex;
	}

	ComponentsDescriptor(ComponentsDescriptor&& rhs) noexcept
	{
		this->region = rhs.region;
		this->momInertia = rhs.momInertia;
		this->convex = rhs.convex;

		rhs.region.clear();
		cv::Moments empty;
		rhs.momInertia = empty;
	}

	ComponentsDescriptor& operator=( const ComponentsDescriptor& rhs)
	{
		region = rhs.region;
		momInertia = rhs.momInertia;
		convex = rhs.convex;
		return *this;
	}

	ComponentsDescriptor& operator=(ComponentsDescriptor&& rhs) noexcept
	{
		this->region = rhs.region;
		this->momInertia = rhs.momInertia;
		this->convex = rhs.convex;

		rhs.region.clear();
		cv::Moments empty;
		rhs.momInertia = empty;
		return *this;
	}
};

struct ImageDescriptors
{
	std::pair<int, int> centroid;
	double Area;
	double perimeter;
	double r_factor;
	double orientation;
	bool convex;

	friend bool operator==(const ImageDescriptors& lhs, const ImageDescriptors& rhs)
	{
		return	(lhs.centroid == rhs.centroid) &&
			(lhs.Area == rhs.Area) &&
			(lhs.perimeter == rhs.perimeter) &&
			(lhs.orientation == rhs.orientation) &&
			(lhs.convex == rhs.convex);
	}

	friend bool operator>(const ImageDescriptors& lhs, const ImageDescriptors& rhs)
	{
		return (lhs.Area > rhs.Area);
	}

	friend bool operator<(const ImageDescriptors& lhs, const ImageDescriptors& rhs)
	{
		return (lhs.Area < rhs.Area);
	}

	friend std::ostream& operator<<(std::ostream& os, const ImageDescriptors& d)
	{
		os << "--------------------------------------------------------------------------------" << std::endl;
		os << "\t\tArea: " << d.Area << std::endl;
		os << "\t\tPerimeter: " << d.perimeter << std::endl;
		os << "\t\tRoundness: " << d.r_factor << std::endl;
		os << "\t\tOrientation: " << d.orientation << std::endl;
		os << "\t\tCentroid: " << "[" << d.centroid.first << "," << d.centroid.second << "]" << std::endl;
		std::string convexHull = d.convex ? "convex" : "not Convex";
		os << "\t\t" << "The region is " << convexHull << std::endl;
		os << "--------------------------------------------------------------------------------" << std::endl;
		return os;
	}
};


using ObjectsCollection = std::vector< ComponentsDescriptor>;
using RegionPoints = std::vector<std::vector<Point> >;
using Descriptors = std::deque< ImageDescriptors >;

class CImageComponentsDescriptorBase
{
public:

	CImageComponentsDescriptorBase(const Mat& img) :original_image(img) {};
	~CImageComponentsDescriptorBase() { original_image.deallocate(); };

	void detectRegions(int mode1 = RETR_CCOMP, int mode2 = CHAIN_APPROX_NONE);
	ObjectsCollection getImageFullInformation()const { return Objects; };

	std::pair<int, int> getCentroid(cv::Moments& momInertia) const;
	double getOrientation(cv::Moments& momInertia) const;
	double getArea(std::vector<cv::Point>& region) const;
	double getPerimeter(std::vector<cv::Point>& region, bool closed = true) const;
	double getRoundNess(std::vector<cv::Point>& region);
	// implement for each type of contour you are using
	virtual void getObjectsInfo() = 0;

	RegionPoints getraw_contourns() { return raw_contourns; };


protected:

	CImageComponentsDescriptorBase(CImageComponentsDescriptorBase&) = delete;
	CImageComponentsDescriptorBase& operator=(CImageComponentsDescriptorBase&) = delete;

	ComponentsDescriptor ImageComponentsDescriptor;
	ObjectsCollection Objects;
	RegionPoints raw_contourns;
	Mat original_image;

};

class CImageComponentsDescriptorNormal : public CImageComponentsDescriptorBase
{
public:
	CImageComponentsDescriptorNormal(const Mat& img) :CImageComponentsDescriptorBase(img) {};
	virtual void getObjectsInfo() override;
};

class CImageComponentsDescriptorHull : public CImageComponentsDescriptorBase
{
public:
	CImageComponentsDescriptorHull(const Mat& img) :CImageComponentsDescriptorBase(img) {};
	virtual void getObjectsInfo() override;
};

class CImageComponentsDescriptorAprox : public CImageComponentsDescriptorBase
{
public:
	CImageComponentsDescriptorAprox(const Mat& img) :CImageComponentsDescriptorBase(img) {};
	virtual void getObjectsInfo() override;
};

namespace image_info
{
	std::stringstream getImageInfoMoments(const Mat& img, int opt);
	Descriptors getImageDescriptors(const Mat& img);
	std::stringstream Apply(CImageComponentsDescriptorBase* base, Mat& img);
	void createCSV(Descriptors& descriptors, std::string fname);
	void drawCountourXY(RegionPoints& raw_contourns);
	std::pair< std::vector<int>, std::vector<int>>
		getImageXY(RegionPoints& raw_contourns);
}






