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

	ComponentsDescriptor& operator=(ComponentsDescriptor&& rhs)
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

using ObjectsCollection = std::vector< ComponentsDescriptor>;
using RegionPoints = std::vector<std::vector<Point> >;

class CImageComponentsDescriptorBase
{
public:

	CImageComponentsDescriptorBase(const Mat& img) :original_image(img) {};
	~CImageComponentsDescriptorBase() { original_image.deallocate(); };
	void detectRegions();
	virtual void getObjectsInfo() = 0;
	ObjectsCollection getImageFullInformation()const { return Objects; };
	std::pair<int, int> getCentroid(cv::Moments& momInertia) const;
	double getArea(std::vector<cv::Point>& region) const;
	bool invalid(			std::pair<int, int>& centroid,
							double& area,
							double area_threshold_min = 0,
							double area_threshold_max = 2000) const;

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

template<typename T>
class CCompare
{
public:

	CCompare(Mat& img1, Mat& img2) :img1{img1}, img2{img2}
	{

	};

	~CCompare()
	{
	}


	void calculateDescriptors();
	
private:

	ObjectsCollection _imag1_descriptions;
	ObjectsCollection _imag2_descriptions;
	
	Mat img1;
	Mat img2;


};

std::stringstream getImageInfoMoments(const Mat& img);

std::stringstream Apply(CImageComponentsDescriptorBase* base, Mat& img);

void test();


