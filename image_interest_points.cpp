#include "image_interest_points.h"


void CImageComponentsDescriptorBase::detectRegions(int mode1, int mode2)
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
    findContours(bw, raw_contourns, mode2, mode1);
}

std::pair<int, int> CImageComponentsDescriptorBase::getCentroid(cv::Moments& momInertia) const
{
    int cx = momInertia.m10 / momInertia.m00;
    int cy = momInertia.m01 / momInertia.m00;
    std::pair<int, int> p(cx, cy);
    return p;
}

double CImageComponentsDescriptorBase::getArea(std::vector<cv::Point>& region) const
{
    return contourArea(region);
}

bool CImageComponentsDescriptorBase::invalid(   std::pair<int, int>& centroid,
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


void CImageComponentsDescriptorNormal::getObjectsInfo()
{
    for (const auto& c : raw_contourns)
    {
        // declare the region
        std::vector<Point> original = c;
        cv::Moments momInertia = cv::moments(cv::Mat(original));
        ImageComponentsDescriptor.region = original;
        ImageComponentsDescriptor.momInertia = momInertia;
        ImageComponentsDescriptor.convex = isContourConvex(original);
        Objects.push_back(ImageComponentsDescriptor);
    }
}

void CImageComponentsDescriptorHull::getObjectsInfo()
{
    for (const auto& c : raw_contourns)
    {
        // declare the region
        std::vector<Point> hull;
        convexHull(c, hull);
        Moments momInertiaHull = cv::moments(cv::Mat(hull));
        ImageComponentsDescriptor.region = hull;
        ImageComponentsDescriptor.momInertia = momInertiaHull;
        ImageComponentsDescriptor.convex = isContourConvex(hull);
        Objects.push_back(ImageComponentsDescriptor);
    }
}


void CImageComponentsDescriptorAprox::getObjectsInfo()
{
    for (const auto& c : raw_contourns)
    {
        // declare the region
        std::vector<Point> Aprox;
        double epsilon = 0.1 * arcLength(c, true);
        approxPolyDP(c, Aprox, epsilon, true);
        cv::Moments momInertiaAprox = cv::moments(cv::Mat(Aprox));
        ImageComponentsDescriptor.region = Aprox;
        ImageComponentsDescriptor.momInertia = momInertiaAprox;
        ImageComponentsDescriptor.convex = isContourConvex(Aprox);
        Objects.push_back(ImageComponentsDescriptor);
    }
}

template<typename T>
void CCompare<T>::calculateDescriptors()
{
    std::unique_ptr<T> descriptor1{ new T{img1} };
    std::unique_ptr<T> descriptor2{ new T{img2} };

    descriptor1->detectRegions();
    descriptor1->getObjectsInfo();
    _imag1_descriptions = descriptor1->getImageFullInformation();

    descriptor2->detectRegions();
    descriptor2->getObjectsInfo();
    _imag2_descriptions = descriptor2->getImageFullInformation();
}

namespace image_info
{
    std::stringstream getImageInfoMoments(const Mat& img, int opt)
    {

        Mat clone = img.clone();

        if (opt == 0)
        {
            std::vector<wxString> choices = {
                                                "Normal algorithm",
                                                "Hull",
                                                "Aproximation"
            };
            wxSingleChoiceDialog dialog(
                NULL,
                "Algorithm for print info",
                "Algorithm for print info",
                static_cast<int>(choices.size()), choices.data());

            dialog.ShowModal();

            CImageComponentsDescriptorBase* base = nullptr;

            wxString algo = dialog.GetStringSelection();
            if (algo == "Normal algorithm")
            {
                base = new CImageComponentsDescriptorNormal(img);
            }
            else
            if (algo == "Hull")
            {
                base = new CImageComponentsDescriptorHull(img);
            }
            else
            if (algo == "Aproximation")
            {
                base = new CImageComponentsDescriptorAprox(img);
            }
            else
            {
                base = new CImageComponentsDescriptorNormal(img);
            }

            return Apply(base, clone);
        }
        else
        {
            CImageComponentsDescriptorNormal* desc =  new CImageComponentsDescriptorNormal(img);
            return Apply(desc, clone);
        }

    }

    std::stringstream Apply(CImageComponentsDescriptorBase* base, Mat& img)
    {

        std::stringstream os;

        base->detectRegions(CHAIN_APPROX_SIMPLE);
        base->getObjectsInfo();
        ObjectsCollection Information = base->getImageFullInformation();
        int objectsIndex = 0;
        for (auto& object : Information)
        {

            double Area = base->getArea(object.region);
            std::pair<int, int> centroid = base->getCentroid(object.momInertia);

            if (base->invalid(centroid, Area, 10, 2000))
            {
                continue;
            }

            os << "Region " << objectsIndex << std::endl;
            os << "--------------------------------------------------------------------------------" << std::endl;
            os << "\t\tArea: " << Area;
            os << "\t\tCentroid: " << "[" << centroid.first << "," << centroid.second << "]" << std::endl;
            std::string convexHull = object.convex ? "convex" : "not Convex";
            os << "\t\t" << "The region is " << convexHull << std::endl;
            os << "--------------------------------------------------------------------------------" << std::endl;
            objectsIndex++;
        }

        if (base != nullptr)
        {
            delete base;
        }

        return os;
    }
}


