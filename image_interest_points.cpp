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

cv::Moments getMomentOfObjectsCollection(InfoRegions& inf)
{
    return inf.momInertia;
}

std::vector<cv::Point> getMomentOfRegionCollection(InfoRegions& inf)
{
    return inf.region;
}