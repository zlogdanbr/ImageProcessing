#pragma once

#include "opcvwrapper.h"
#include "savekernel.h"
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
	double HuMoments[7];

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

	std::string getHuhMomentsLine(Mat& img);
	std::pair<int, int> getCentroid(cv::Moments& momInertia);
	std::string convertWxStringToString(const wxString wsx);

	double getArea(std::vector<cv::Point>& region);
	double getPerimeter(std::vector<cv::Point>& region, bool closed);
	double getRoundNess(std::vector<cv::Point>& region);
	double getOrientation(cv::Moments& momInertia);
	void getHuMoments(std::vector<cv::Point>& region, double* huh);
	std::string getHuhMomentsLine(Mat& img);
	
	std::string loadDescriptorFile();

	int readCSV2(	std::vector<std::vector<double>>& obs,
					int nfields,
					bool ignoreheader,
					std::string& filename);


	ObjectsCollection getContournInfo(const Mat& img);


}

namespace fast_algo
{
	void ApplyAndCompareFAST(std::vector<Mat>& images,
		std::vector<std::string>& filenames);

	std::vector < cv::KeyPoint >  ApplyFAST(const Mat& img);
}

namespace sift_algo
{
	/*
	*		Creates a csv file with with sif descriptors called somewhere
	*		else
	*/
	void createCSV(std::vector < cv::KeyPoint >& descriptors, std::string fname);

	Mat ApplyAndCompareSIFT(std::vector<Mat>& images,
		std::vector<std::string>& filenames);

	std::vector < cv::KeyPoint >  ApplySift(const Mat& img, Mat& descriptors);

	Mat getMatchedImage(	Mat& descriptor1,
							Mat& descriptor2,
							std::vector < cv::KeyPoint >& kp1,
							std::vector < cv::KeyPoint >& kp2,
							Mat& img1,
							Mat& img2,
							int option = 0);
}

namespace template_matching
{
	std::pair<Mat, Mat>  ApplyTemplateMatching(const Mat&, Mat&);

	template<typename F, typename...Args>
	Mat ApplyTemplateMatchingFull(	const Mat& BigImage, 
									std::vector<Mat>& templ, 
									int mode,
									F& f,
									Args&&... args);

	namespace canny_matching
	{
		Mat ApplyTemplateMatchingFull_TM_SQDIFF(const Mat& BigImage, std::vector<Mat>& templ,int t1, int t2);
		Mat ApplyTemplateMatchingFull_TM_SQDIFF_NORMED(const Mat& BigImage, std::vector<Mat>& templ, int t1, int t2);
		Mat ApplyTemplateMatchingFull_TM_CCORR(const Mat& BigImage, std::vector<Mat>& templ, int t1, int t2);
		Mat ApplyTemplateMatchingFull_TM_CCORR_NORMED(const Mat& BigImage, std::vector<Mat>& templ, int t1, int t2);
		Mat ApplyTemplateMatchingFull_TM_CCOEFF(const Mat& BigImage, std::vector<Mat>& templ, int t1, int t2);
		Mat ApplyTemplateMatchingFull_TM_CCOEFF_NORMED(const Mat& BigImage, std::vector<Mat>& templ, int t1, int t2);
	}

}









