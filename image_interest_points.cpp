#include "image_interest_points.h"


RegionPoints detectRegions(const Mat& img )
{
    Mat src = img.clone();
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
    RegionPoints contours;
    findContours(bw, contours, RETR_LIST, CHAIN_APPROX_NONE);
    return contours;

}

ObjectsCollection getObjectsInfo(RegionPoints& raw_contourns)
{
    ObjectsCollection Objects;
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

        ImageComponentsDescriptor inf;

        inf.region = original;
        inf.regionHull = hull;
        inf.regionAprox = Aprox;

        inf.momInertia = momInertia;
        inf.momInertiaHull = momInertiaHull;
        inf.momInertiaAprox = momInertiaAprox;

        inf.convex = isContourConvex(original);
        inf.convexHull = isContourConvex(hull);
        inf.convexAprox = isContourConvex(Aprox);

        Objects.push_back(inf);
    }

    return Objects;
}


std::vector<cv::Point> getMomentOfRegionCollection(ImageComponentsDescriptor& inf)
{
    return inf.region;
}

std::pair<int, int> 
getCentroid(cv::Moments& momInertia)
{
    int cx = momInertia.m10 / momInertia.m00;
    int cy = momInertia.m01 / momInertia.m00;
    std::pair<int, int> p(cx, cy);
    return p;
}

double
getArea(std::vector<cv::Point>& region)
{
    return contourArea(region);
}

bool invalid(   std::pair<int, int>& centroid,
                double& area,
                double area_threshold_min,
                double area_threshold_max)
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

    RegionPoints contours = detectRegions(img);
    ObjectsCollection Objects = getObjectsInfo(contours);
    std::stringstream os;

    int cnt = 0;
    for (const auto& object : Objects)
    {
        std::vector<cv::Point> reg = object.region;
        cv::Moments momInertia = object.momInertia;
        std::pair<int, int> Centroid = getCentroid(momInertia);
        double area = contourArea(object.region);

        std::string _convex = object.convex ? "Convex" : "Not Convex";

        if (invalid(Centroid, area, 10,2000) == false)
        {
            os << "--------------------------------------------------------" << std::endl;
            os << "Region " << cnt << std::endl;
            os << "--------------------------------------------------------" << std::endl;
            os << "Centroid: (" << Centroid.first << "," << Centroid.second << ")" << std::endl;
            os << "Area: " << area << std::endl;
            os << _convex << std::endl;
            cnt++;
        }
        else
        {
            continue;
        }
    }

    return os;

}
