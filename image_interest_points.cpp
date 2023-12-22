#include "image_interest_points.h"


std::vector<std::vector<Point> > detectRegions(const Mat& img )
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
    std::vector<std::vector<Point> > contours;
    findContours(bw, contours, RETR_LIST, CHAIN_APPROX_NONE);
    return contours;

}

ObjectsCollection getObjectsInfo(std::vector<std::vector<Point> >& raw_contourns)
{
    ObjectsCollection Objects;
    for (const auto& c : raw_contourns)
    {
        std::vector<cv::Point> hull;
        cv::convexHull(c, hull);
        cv::Moments mom = cv::moments(cv::Mat(c));

        InfoRegions inf;
        inf.region = hull;
        inf.momInertia = mom;
        Objects.push_back(inf);
    }

    return Objects;
}


std::vector<cv::Point> getMomentOfRegionCollection(InfoRegions& inf)
{
    return inf.region;
}

std::pair<int, int> getCentroid(cv::Moments& momInertia)
{
    int cx = momInertia.m10 / momInertia.m00;
    int cy = momInertia.m01 / momInertia.m00;
    std::pair<int, int> p(cx, cy);
    return p;
}

std::stringstream getImageInfoMoments(const Mat& img)
{

    std::vector<std::vector<Point> > contours = detectRegions(img);
    ObjectsCollection Objects = getObjectsInfo(contours);
    std::stringstream os;

    int cnt = 0;
    for (const auto& object : Objects)
    {
        std::vector<cv::Point> reg = object.region;
        cv::Moments momInertia = object.momInertia;
        std::pair<int, int> Centroid = getCentroid(momInertia);
        double area = contourArea(object.region);

        if (invalid(Centroid, area, 10,2000) == false)
        {
            os << "--------------------------------------------------------" << std::endl;
            os << "Region " << cnt << std::endl;
            os << "--------------------------------------------------------" << std::endl;
            os << "Centroid: (" << Centroid.first << "," << Centroid.second << ")" << std::endl;
            os << "Area: " << area << std::endl;
            cnt++;
        }
        else
        {
            continue;
        }
    }

    return os;

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