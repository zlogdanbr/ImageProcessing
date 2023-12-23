#include "image_interest_points.h"


void CImageComponentsDescriptor::detectRegions()
{
    Mat src = original_image.clone();
    // Convert image to grayscale
    Mat gray;

    if (isGrayScaleImage(src) == false)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src.clone();
    }

    // Convert image to binary
    Mat bw;
    threshold(gray, bw, 50, 255, THRESH_BINARY | THRESH_OTSU);
    // Find all the contours in the thresholded image
    findContours(bw, raw_contourns, RETR_LIST, CHAIN_APPROX_NONE);
}

void CImageComponentsDescriptor::getObjectsInfo()
{    
    for (const auto& c : raw_contourns)
    {
        // declare the region
        std::vector<Point> original = c;
        std::vector<Point> hull;
        std::vector<Point> Aprox;

        double epsilon = 0.1 * arcLength(c, true);
        approxPolyDP(original, Aprox, epsilon, true);
        cv::convexHull(original, hull);

        cv::Moments momInertiaHull = cv::moments(cv::Mat(hull));
        cv::Moments momInertia = cv::moments(cv::Mat(original));
        cv::Moments momInertiaAprox = cv::moments(cv::Mat(Aprox));

        ImageComponentsDescriptor.region = original;
        ImageComponentsDescriptor.regionHull = hull;
        ImageComponentsDescriptor.regionAprox = Aprox;

        ImageComponentsDescriptor.momInertia = momInertia;
        ImageComponentsDescriptor.momInertiaHull = momInertiaHull;
        ImageComponentsDescriptor.momInertiaAprox = momInertiaAprox;
    
        ImageComponentsDescriptor.convex = isContourConvex(original);
        ImageComponentsDescriptor.convexHull = isContourConvex(hull);
        ImageComponentsDescriptor.convexAprox = isContourConvex(Aprox);

        Objects.push_back(ImageComponentsDescriptor);
    }
}

std::pair<int, int> CImageComponentsDescriptor::getCentroid(cv::Moments& momInertia) const
{
    int cx = momInertia.m10 / momInertia.m00;
    int cy = momInertia.m01 / momInertia.m00;
    std::pair<int, int> p(cx, cy);
    return p;
}

double CImageComponentsDescriptor::getArea(std::vector<cv::Point>& region) const
{
    return contourArea(region);
}

bool CImageComponentsDescriptor::invalid(   std::pair<int, int>& centroid,
                                            double& area,
                                            double area_threshold_min,
                                            double area_threshold_max) const
{
    if (area <= area_threshold_min || area >= area_threshold_max)
    {
        return true;
    }

    if (centroid.first >= INT_MAX || centroid.first <= INT_MIN)
    {
        return true;
    }

    if (centroid.second >= INT_MAX || centroid.second <= INT_MIN)
    {
        return true;
    }

    return false;

}


std::stringstream getImageInfoMoments(const Mat& img)
{
    std::stringstream os;
    CImageComponentsDescriptor Descriptor(img);
    Descriptor.detectRegions();
    Descriptor.getObjectsInfo();
    ObjectsCollection Information = Descriptor.getImageFullInformation();
    int objectsIndex = 0;
    for (auto& object : Information)
    {
        os << "Region " << objectsIndex << std::endl;
        os << "--------------------------------------------------------------------------------" << std::endl;
        
        os << "\tRegular Contorn:" << std::endl;
        os << "\t\tArea: " << Descriptor.getArea( object.region);
        std::pair<int, int> centroid = Descriptor.getCentroid(object.momInertia);
        os << "\t\tCentroid: "  << "[" << centroid.first << "," << centroid.second << "]" << std::endl;
        std::string convex = object.convex ? "convex" : "not Convex";
        os << "\t\t" << "The region is " << convex << std::endl;

        os << "\tHull Contorn:" << std::endl;
        os << "\t\tArea: " << Descriptor.getArea(object.regionHull);
        std::pair<int, int> centroidHull = Descriptor.getCentroid(object.momInertiaHull);
        os << "\t\tCentroid: " << "[" << centroidHull.first << "," << centroidHull.second << "]" << std::endl;
        std::string convexHull = object.convexHull ? "convex" : "not Convex";
        os << "\t\t" << "The region is " << convexHull << std::endl;

        os << "\tAproximated Contorn:" << std::endl;
        os << "\t\tArea: " << Descriptor.getArea(object.regionAprox);
        std::pair<int, int> centroidAprox = Descriptor.getCentroid(object.momInertiaAprox);
        os << "\t\tCentroid: " << "[" << centroidAprox.first << "," << centroidAprox.second << "]" << std::endl;
        std::string convexAprox = object.convexAprox ? "convex" : "not Convex";
        os << "\t\t" << "The region is " << convexAprox << std::endl;

        os << "--------------------------------------------------------------------------------" << std::endl;
        objectsIndex++;
    }

    return os;
}
