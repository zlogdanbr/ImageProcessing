#include "image_interest_points.h"

#include "filesys.h"
#include <fstream>


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

    Mat canny_output;
    Canny(gray, canny_output, 50, 255);

    // Find all the contours in the thresholded image
    findContours(canny_output, raw_contourns, mode1, mode2);
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

double CImageComponentsDescriptorBase::getPerimeter(std::vector<cv::Point>& region, bool closed) const
{
    return arcLength(region, closed);
}

double CImageComponentsDescriptorBase::getRoundNess(std::vector<cv::Point>& region)
{
    double Area = getArea(region);
    double Perimeter = getPerimeter(region);

    return 4*CV_PI*( Area/pow(Perimeter,2));
}

double CImageComponentsDescriptorBase::getOrientation(cv::Moments& momInertia) const
{
    double u11 = momInertia.m11;
    double u20 = momInertia.m20;
    double u02 = momInertia.m02;

    double factor = ( 2 * u11 )/ (u20 - u02);
    double angle =  0.5 * atan(factor);

    double degrees = angle * (180.0 / CV_PI);

    if (degrees < 0)
    {
        degrees = 180 + degrees;
    }
    return degrees;
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

namespace image_info
{

 

    Descriptors getImageDescriptors(const Mat& img)
    {

        Descriptors out;

        CImageComponentsDescriptorHull hull(img);

        hull.detectRegions(CHAIN_APPROX_SIMPLE);
        hull.getObjectsInfo();
        ObjectsCollection Information = hull.getImageFullInformation();
        int objectsIndex = 0;
        for (auto& object : Information)
        {

            double Area = hull.getArea(object.region);
            double perimeter = arcLength(object.region, true);
            double r_factor = hull.getRoundNess(object.region);
            double orientation = hull.getOrientation(object.momInertia);

            std::pair<int, int> centroid = hull.getCentroid(object.momInertia);

            if (Area < 1e2 || 1e5 < Area) continue;

            ImageDescriptors d;

            d.Area = Area;
            d.perimeter = perimeter;
            d.r_factor = r_factor;
            d.orientation = orientation;
            d.convex = object.convex;
            d.centroid = centroid;

            double Huh[7];
            for (auto& h : Huh)
            {
                h = 0;
            }

            HuMoments(object.momInertia, Huh);

            for (int i = 0; i < 7; i++)
            {
                d.HuMoments[i] = -1 * copysign(1.0, Huh[i]) * log10(abs(Huh[i]));
            }

            out.emplace_back(d);
        }

        return out;
    }


    void createCSV(Descriptors& descriptors, std::string fname)
    {
        std::sort(descriptors.begin(), descriptors.end());

        std::ofstream myfile(fname);
        int i = 0;
        if (myfile.is_open())
        {
            for (const auto& descriptor : descriptors)
            {
                if (i == 0)
                {
                    myfile << "Area,Perimeter,roundness,orientation,cx,cy,h0,h1,h2,h3,h4,h5,h6" << std::endl;
                    i++;
                    continue;
                }
                std::stringstream s;
                s << descriptor.Area << "," <<
                    descriptor.perimeter << "," <<
                    descriptor.r_factor << "," <<
                    descriptor.centroid.first << "," <<
                    descriptor.centroid.second << "," <<
                    descriptor.orientation << ",";

                for (int i = 0; i < 7; i++)
                {
                    if (i == 6)
                    {
                        s << descriptor.HuMoments[i] << std::endl;
                    }
                    else
                    {
                        s << descriptor.HuMoments[i] << ",";
                    }
                    
                }

                myfile << s.str();

            }
            myfile.close();
        }
    }

    void createCSV(std::vector < cv::KeyPoint >& descriptors, std::string fname)
    {
        std::ofstream myfile(fname);
        int i = 0;
        if (myfile.is_open())
        {
            for (const auto& descriptor : descriptors)
            {
                if (i == 0)
                {
                    myfile << "x,y,size,angle,response,octave,class_id" << std::endl;
                    i++;
                    continue;
                }
                                
                // write fields to s
                std::stringstream s;
                s << descriptor.pt.x << ",";
                s << descriptor.pt.y << ",";
                s << descriptor.size << ",";
                s << descriptor.angle << ",";
                s << descriptor.response << ",";
                s << descriptor.octave << ",";
                s << descriptor.class_id << "," << std::endl;
                myfile << s.str();
            }
            myfile.close();
        }
    }
}

namespace fast_algo
{
    std::vector < cv::KeyPoint >  ApplyFAST(const Mat& img)
    {
        Mat gray = convertograyScale(img);
        cv::Ptr<cv::FeatureDetector> ptrDetector; // generic detector
        ptrDetector = cv::FastFeatureDetector::create(80);
        std::vector<cv::KeyPoint> keypoints;

        // Keypoint detection
        ptrDetector->detect(gray, keypoints);

        return keypoints;
    }

    void ApplyAndCompareFAST(   std::vector<Mat>& images,
                                std::vector<std::string>& filenames)
    {
    }

}


namespace sift_algo
{
    std::vector < cv::KeyPoint> ApplySift(const Mat& img, Mat& descriptors)
    {
        Mat gray = convertograyScale(img);
        //auto sift = SIFT::create();
        cv::Ptr<cv::Feature2D> sift = SIFT::create();
        std::vector<cv::KeyPoint> keypoints;
        sift->detect(gray, keypoints);
        sift->detectAndCompute(gray, noArray(), keypoints, descriptors);
        return keypoints;
    }

    Mat getMatchedImage(    Mat& descriptor1, 
                            Mat& descriptor2, 
                            std::vector < cv::KeyPoint >&  kp1,
                            std::vector < cv::KeyPoint >&  kp2,
                            Mat& img1,
                            Mat& img2,
                            int option)
    {
        Mat result;
        std::vector< DMatch > matches;
        if (option == 0)
        {
            cv::BFMatcher matcher(cv::NORM_L2, true);
            

            matcher.match(descriptor1, descriptor2, matches);

            // extract the show_matches best matches
            int show_matches = min(kp1.size(), kp2.size());
            show_matches = min(show_matches, 10);
            std::nth_element(matches.begin(), matches.begin() + show_matches, matches.end());
            matches.erase(matches.begin() + show_matches, matches.end());
        }
        else
        if (option == 1)
        {
            std::vector<std::vector<cv::DMatch>> matches2D;
            cv::BFMatcher matcher(NORM_L2);
            matcher.knnMatch(descriptor1, descriptor2, matches2D, 2); // find the k best match

            double ratio = 0.85;
            std::vector<std::vector<cv::DMatch>>::iterator it;
            for (it = matches2D.begin(); it != matches2D.end(); ++it) 
            {
                // first best match/second best match
                if ((*it)[0].distance / (*it)[1].distance < ratio)
                {
                    // it is an acceptable match
                    matches.push_back((*it)[0]);
                }
            }
        }
        else
        if (option == 2)
        {

            double maxDist = 0.4;

            std::vector<std::vector<cv::DMatch>> matches2D;
            cv::BFMatcher matcher(NORM_L2);
            // maximum acceptable distance
            // between the 2 descriptors
            matcher.radiusMatch(descriptor1, descriptor2, matches2D, maxDist);

            matches = matches2D[1];
        }

        drawMatches(
            img1,        // InputArray 	img1,
            kp1,         // const std::vector< KeyPoint > & 	keypoints1,
            img2,        // InputArray 	img2,
            kp2,         // const std::vector< KeyPoint > & 	keypoints2,
            matches,     // const std::vector< DMatch > & 	matches1to2,
            result,      // InputOutputArray 	outImg
            1            // const int 	matchesThickness
        );

        return result;
    }

    void ApplyAndCompareSIFT(   std::vector<Mat>& images, 
                                std::vector<std::string>& filenames)
    {

        if (images.size() != 2)
        {
            return;
        }

        Mat& img1 = images[0];
        Mat& img2 = images[1];

        int width_01 = img1.size().width;
        int height_01 = img1.size().height;

        resize(img2, img2, Size(width_01, height_01), INTER_LINEAR);

        Mat descriptor1;
        Mat descriptor2;

        std::vector < cv::KeyPoint >  kp1 = ApplySift(img1, descriptor1);
        std::vector < cv::KeyPoint >  kp2 = ApplySift(img2, descriptor2);

        std::stringstream os1;
        std::stringstream os2;

        std::string f = createFolderAtHomeUser("\\dimage");
        os1 << f << "\\" << "image_sift_" << getOnlyNameNoExt(filenames[0]) <<  ".csv";
        image_info::createCSV(kp1, os1.str());
        os2 << f << "\\" << "image_sift_" << getOnlyNameNoExt(filenames[1]) << ".csv";
        image_info::createCSV(kp2, os2.str());

        Mat result = getMatchedImage(descriptor1, descriptor2, kp1, kp2, img1, img2);
        
        showImage(result, "Result");

    }
}

