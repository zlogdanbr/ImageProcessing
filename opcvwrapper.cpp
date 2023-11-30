#include "opcvwrapper.h"
#include "dmath.h"


std::vector< Eigen::MatrixXd > convertOpenCVToEigen(Mat& OpenCVImage)
{
    std::vector< Eigen::MatrixXd > ThreeChannels;

    Eigen::MatrixXd channel1;
    Eigen::MatrixXd channel2;
    Eigen::MatrixXd channel3;

    unsigned char* input = static_cast<unsigned char*>(OpenCVImage.data);

    if (isGrayScaleImage(OpenCVImage))
    {
        for (int j = 0; j < OpenCVImage.rows; j++)
        {
            for (int i = 0; i < OpenCVImage.cols; i++)
            {
                channel1 << input[OpenCVImage.step * j + i];
                channel2 << input[OpenCVImage.step * j + i + 1];
                channel3 << input[OpenCVImage.step * j + i + 2];
            }
        }
        ThreeChannels.push_back(channel1);
    }
    else
    {
        for (int j = 0; j < OpenCVImage.rows; j++)
        {
            for (int i = 0; i < OpenCVImage.cols; i++)
            {
                channel1 << input[OpenCVImage.step * j + i];
                channel2 << input[OpenCVImage.step * j + i + 1];
                channel3 << input[OpenCVImage.step * j + i + 2];
            }
        }
        ThreeChannels.push_back(channel1);
        ThreeChannels.push_back(channel2);
        ThreeChannels.push_back(channel3);
    }

    return ThreeChannels;
}

// https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
void drawCirclesAtImgFromRoi(Mat& img, Rect& roi)
{
    Point Mycenter(roi.x + roi.width / 2,
        roi.y + roi.height / 2);

    int radius = cvRound((roi.width + roi.height) * 0.25);
    circle(img, Mycenter, radius, Scalar(255, 0, 0), 4);

}

// https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
void drawSquaresAtImgFromRoi(Mat& img, Rect& roi)
{
    Scalar color = Scalar(255, 0, 0);
    rectangle(img,
        Point(cvRound(roi.x * 1),
            cvRound(roi.y * 1)),
        Point(cvRound((roi.x + roi.width - 1) * 1),
            cvRound((roi.y + roi.height - 1) * 1)),
        color,
        3,
        8,
        0);

}

Mat InvertImage(const Mat& img)
{
    Mat out;
    bitwise_not(img, out);
    return out;
}

// https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
bool loadImage(const std::string& image_path, Mat& img)
{
    img = imread(image_path);
    if (img.empty())
    {
        return false;
    }
    return true;
}

bool isGrayScaleImage(const Mat& img)
{
    return  (img.channels() == 1) ? true : false;
}

Mat flipImageHorizontal(const Mat& img)
{
    cv::Mat result; 
    cv::flip(img, result, 1); // positive for horizontal
    return result;
}

Mat flipImageVertical(const Mat& img)
{
    cv::Mat result; 
    cv::flip(img, result, 0); // positive for horizontal
    return result;
}

Mat flipImage(const Mat& img)
{
    cv::Mat result; 
    cv::flip(img, result, -1); // positive for horizontal
    return result;
}


// https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
bool saveImage(const std::string& image_path, Mat& img)
{
    return imwrite(image_path, img);
}

// https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
void showImage(const Mat& img, const std::string& title)
{
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, 400, 400);
    imshow(title, img);
}

// https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
Mat convertRectoImg(Rect& r, Mat& img)
{
    Mat roi = Mat(img, r);
    return roi;
}

// https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
Mat convertograyScale(const Mat& img)
{

    if (isGrayScaleImage(img) )
    {
        return img;
    }

    Mat grayscaleimage;

    //change the color image to grayscale image
    cvtColor(img, grayscaleimage, COLOR_BGR2GRAY);
    return grayscaleimage;
}

Mat getBinaryImage(const Mat& img)
{
    // Create binary image from source image
    Mat bw;
    cvtColor(img, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    return bw;
}

//https://docs.opencv.org/4.x/d4/d1b/tutorial_histogram_equalization.html
Mat equalizeGrayImage(const Mat& img)
{
    try
    {
        Mat hist_equalized_image;
        equalizeHist(img, hist_equalized_image);
        return hist_equalized_image;
    }
    catch(...)
    {
        Mat out = equalizeColorImage(img);
        return out;
    }

}

//https://docs.opencv.org/4.x/d4/d1b/tutorial_histogram_equalization.html
Mat equalizeColorImage(const Mat& img)
{
    Mat hist_equalized_image;
    cvtColor(img, hist_equalized_image, COLOR_BGR2YCrCb);
    std::vector<Mat> vec_channels;
    split(hist_equalized_image, vec_channels);

    //Equalize the histogram of only the Y channel 
    equalizeHist(vec_channels[0], vec_channels[0]);

    //Merge 3 channels in the vector to form the color image in YCrCB color space.
    merge(vec_channels, hist_equalized_image);

    //Convert the histogram equalized image from YCrCb to BGR color space again
    cvtColor(hist_equalized_image, hist_equalized_image, COLOR_YCrCb2BGR);
    return hist_equalized_image;
}

//https://docs.opencv.org/4.x/d4/d1b/tutorial_histogram_equalization.html
std::vector<Mat> splitChannel(Mat& img)
{
    Mat colorrgb;
    cvtColor(img, colorrgb, COLOR_BGR2YCrCb);
    std::vector<Mat> vec_channels;
    split(colorrgb, vec_channels);
    return vec_channels;
}

// https://docs.opencv.org/3.4/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html
Mat blurImageSmooth(const Mat& img, int kernel_size)
{
    Mat Blurred;
    blur(img, Blurred, Size(kernel_size, kernel_size));
    return Blurred;
}

Mat MedianImageSmooth(const Mat& img, int kernel_size)
{
    Mat MedianI;
    medianBlur(img, MedianI, kernel_size);
    return MedianI;
}

// https://docs.opencv.org/3.4/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html
Mat GaussianImageSmooth(const Mat& img, int kernel_size)
{
    Mat Blurred;
    GaussianBlur(img, Blurred, Size(kernel_size, kernel_size), 0);
    return Blurred;
}

Mat GaussianImageSmoothExtended(    const Mat& img, 
                                    int kernel_size,
                                    double sigmaX,
                                    double sigmaY
                                )
{
    Mat Blurred;
    GaussianBlur(   
                    img, 
                    Blurred, 
                    Size(kernel_size, kernel_size), 
                    sigmaX, // sigmaX standard deviation in X direction
                    sigmaY // sigmaY standard deviation in Y directiom 
                       //if sigmaY is zero, it is set to be equal to sigma                    
                );
    return Blurred;
}

Mat ApplyThreShold(const Mat& img)
{
    cv::Mat out;
    cv::threshold(img,
        out,
        1,
        255,
        cv::THRESH_BINARY);
    return out;
}

// https://docs.opencv.org/3.4/d5/db5/tutorial_laplace_operator.html
Mat ApplyLaplacian(const Mat& src)
{
    // Declare the variables we are going to use
    Mat src_gray, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    src_gray = convertograyScale(src);

    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(src_gray, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);

       
    Mat abs_dst;
    Laplacian(  src_gray, 
                dst, 
                ddepth, 
                kernel_size, 
                scale, 
                delta, 
                BORDER_DEFAULT);

    // converting back to CV_8U
    convertScaleAbs(dst, abs_dst);
    return abs_dst;
}

Mat ApplyLaplacianExtended(const Mat& src, int kernel_size, int scale, int delta, int ddepth)
{
    // Declare the variables we are going to use
    Mat src_gray, dst;

    src_gray = convertograyScale(src);

    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(src_gray, src_gray, Size(3, 3), 0, 0, BORDER_DEFAULT);


    Mat abs_dst;
    Laplacian(src_gray,
        dst,
        ddepth,
        kernel_size,
        scale,
        delta,
        BORDER_DEFAULT);

    // converting back to CV_8U
    convertScaleAbs(dst, abs_dst);
    return abs_dst;

}


// https://docs.opencv.org/4.x/d4/dbd/tutorial_filter_2d.html
Mat ApplyCustomKernel(const Mat& img, Mat& kernel)
{
    Mat final;

    //  src: Source image
    //  dst : Destination image
    //  ddepth : The depth of dst.A negative value(such as −1) indicates that the depth is the same as the source.
    //  kernel : The kernel to be scanned through the image
    //  anchor : The position of the anchor relative to its kernel.The location Point(-1, -1) indicates the center by default.
    //  delta : A value to be added to each pixel during the correlation.By default it is 0
    //  BORDER_DEFAULT : We let this value by default (more details in the following tutorial)

    // image,result,image.depth(),kernel
    filter2D(   img,
                final,
                img.depth(),
                kernel);
    return final;
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 188 ]
----------------------------------------------------------------------------------------------*/
Mat ApplySobelX(const Mat& img, int kernel_size)
{
    Mat sobelX;
    cv::Sobel(img, // input
        sobelX, // output
        CV_8U, // image type
        1, 0, // kernel specification
        kernel_size, // size of the square kernel
        0.4, 128); // scale and offset
    return sobelX;
}


Mat ApplySobelXExtended(    const Mat& img,
                    int image_type, 
                    int depth, 
                    int type,
                    double delta,
                    int kernel_size)
{
    Mat sobelX;
    cv::Sobel(      img,            // input image
                    sobelX,         // output
                    CV_8U,          // ddepth
                    1,              // dx order of the derivative x
                    0,              // order of the derivative y.
                    kernel_size,    // size of the square kernel
                    delta,          // delta value that is added to the results prior to storing them in dst
                    128             // #BorderTypes. #BORDER_WRAP is not supported.
            ); 
    return sobelX;
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 188 ]
----------------------------------------------------------------------------------------------*/
Mat ApplySobelY(const Mat& img, int kernel_size)
{
    Mat sobelY;

    cv::Sobel(img, // input
        sobelY, // output
        CV_8U, // image type
        0, 1, // kernel specification
        kernel_size, // size of the square kernel
        0.4, 128); // scale and offset

    return sobelY;
}

Mat ApplySobelYExtended(const Mat& img,
                int image_type,
                int depth,
                int type,
                double delta,
                int kernel_size)
{
    Mat sobelY;

    cv::Sobel(img,      // input image
        sobelY,         // output
        CV_8U,          // ddepth
        0,              // dx order of the derivative x
        1,              // order of the derivative y.
        kernel_size,    // size of the square kernel
        delta,          // delta value that is added to the results prior to storing them in dst
        128             // #BorderTypes. #BORDER_WRAP is not supported.
    );

    return sobelY;
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 189 ]
Note: The author uses CV_16U so it will not work unless you match with the above calls
----------------------------------------------------------------------------------------------*/
Mat ApplySobel(const Mat& img, int kernel_size)
{
    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Mat src_gray;
    src_gray = convertograyScale(img);
    Mat sobelX = ApplySobelX(src_gray, kernel_size);
    Mat sobelY = ApplySobelY(src_gray, kernel_size);

    // Compute norm of Sobel
    cv::Sobel(src_gray, sobelX, CV_8U, 1, 0);
    cv::Sobel(src_gray, sobelY, CV_8U, 0, 1);
    cv::Mat sobel;
    //compute the L1 norm
    sobel = abs(sobelX) + abs(sobelY);
    return sobel;
}

Mat ApplySobelExtended( const Mat& img,
                int image_type,
                int depth,
                int type,
                double delta,
                int kernel_size)
{
    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Mat src_gray;
    src_gray = convertograyScale(img);
    Mat sobelX = ApplySobelXExtended(src_gray, image_type, depth, type, delta, kernel_size);
    Mat sobelY = ApplySobelYExtended(src_gray, image_type, depth, type, delta, kernel_size);

    // Compute norm of Sobel
    cv::Sobel(src_gray, sobelX, CV_8U, 1, 0);
    cv::Sobel(src_gray, sobelY, CV_8U, 0, 1);
    cv::Mat sobel;
    //compute the L1 norm
    sobel = abs(sobelX) + abs(sobelY);
    return sobel;
}


//https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
Mat ApplyHoughTransform(const Mat& img, int opt)
{
    Mat dst;
    Mat cdst;
    Mat cdstP;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    // Standard Hough Line Transform
    std::vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
    }
    // Probabilistic Line Transform
    std::vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++)
    {
        Vec4i l = linesP[i];
        line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }

    if ( opt == 0 )
    {
        // Standard Hough Line Transform
        return cdst;
    }
    else
    {
        // Probabilistic Line Transform
        return cdstP;
    }

}

Mat ApplyHoughTransformRegular(const Mat& img)
{
    return ApplyHoughTransform(img, 0);
}

Mat ApplyHoughTransformReProbabilistic(const Mat& img)
{
    return ApplyHoughTransform(img, 1);
}
Mat Sharpening(const Mat& img)
{
    Mat Iconv = 0.5 * ApplyLaplacian(img);
    Mat igray = convertograyScale(img);
    return igray - Iconv;
}

Mat Unsharp(const Mat& img)
{
    Mat Iconv = GaussianImageSmooth(img,3);
    Mat igray = convertograyScale(img);
    return (1.5) * igray - 0.5 * Iconv;
}

Mat ApplyErode(const Mat& img)
{
    cv::Mat eroded; // the destination image
    cv::erode(img, eroded, cv::Mat());
    return eroded;
}

Mat ApplyDilate(const Mat& img)
{
    cv::Mat dilated; // the destination image
    cv::dilate(img, dilated, cv::Mat());
    return dilated;
}

Mat ApplyMorphGradient(const Mat& img)
{

    cv::Mat result;
    cv::morphologyEx(img,
        result,
        cv::MORPH_GRADIENT,
        cv::Mat());

        return result;
}

Mat ApplyTopHatAlgo(const Mat& img)
{
    cv::Mat result;
    cv::Mat element7(7, 7, CV_8U, cv::Scalar(1));
    cv::morphologyEx(img, result, cv::MORPH_BLACKHAT, element7);
    return result;
}




/**
    This function is the one I use to test algorithms I am studing
    and applying them together with other filters.
*/
Mat workingAlgorithm(const Mat& image)
{
    cv::Mat imgclone1;

    // Convert to gray scale
    imgclone1 = convertograyScale(image);

    GaussianImageSmoothExtended(imgclone1, 5, 100, 25);

    return imgclone1;
}


/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 239 ]
See page [245 ] for the math
----------------------------------------------------------------------------------------------*/
Mat detectCornersHarrisAlgoFull(const Mat& image,
    int neighborhood_size,
    int aperture_size,
    double threshold,
    double Harris_parameter
)
{
    // Detect Harris Corners
    cv::Mat cornerStrength;
    cv::Mat imgclone;
    imgclone = convertograyScale(image);

    cv::cornerHarris(imgclone, // input image
        cornerStrength, // image of cornerness
        neighborhood_size, // neighborhood size
        aperture_size, // aperture size
        Harris_parameter); // Harris parameter

    cv::Mat harrisCorners;

    // threshold the corner strengths
    cv::threshold(cornerStrength,
        harrisCorners,
        threshold,
        255,
        cv::THRESH_BINARY);

    return harrisCorners;
}

Mat detectCornersHarris(const Mat& image)
{
    return detectCornersHarrisAlgoFull(image, 3, 3, 0.0001, 0.01);
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 250 ]
----------------------------------------------------------------------------------------------*/
Mat detectFastKeyPoints(const Mat& image)
{
    cv::Mat imgclone;

    imgclone = convertograyScale(image);

    // vector of keypoints
    std::vector<cv::KeyPoint> keypoints;

    // FAST detector with a threshold of 40
    cv::Ptr<cv::FastFeatureDetector> ptrFAST = cv::FastFeatureDetector::create(40);
    // detect the keypoints
    ptrFAST->detect(imgclone, keypoints);

    cv::drawKeypoints(image, // original image
        keypoints, // vector of keypoints
        imgclone, // the output image
        cv::Scalar(255, 255, 255), // keypoint color
        cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);// drawing flag

    return imgclone;
}

TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints)
{
    TargetPoints out;
    std::vector<int> x;
    std::vector<int> y;

    for (const auto& point : AbstractPoints)
    {
        x.push_back(point.first);
        y.push_back(point.second);
    }

    out = make_pair(x, y);

    return out;

}

AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints)
{
    AbstractRegion  AbstractPoints;
    for (const auto& p : keypoints)
    {
        int ix = static_cast<int>(p.pt.x);
        int iy = static_cast<int>(p.pt.y);
        std::pair<int, int> pr{ ix,iy };
        AbstractPoints.push_back(pr);
    }
    return AbstractPoints;
}

void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE r, UBYTE g, UBYTE b, bool blank_bgr)
{
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            Vec3b color = img.at<Vec3b>(Point(x, y));

            std::pair<int, int> p{ x,y };
            auto it = std::find(abstract_region.cbegin(), abstract_region.cend(), p);

            if (it != abstract_region.end())
            {
                color[0] = r;
                color[1] = g;
                color[2] = b;
            }
            else
            {
                if (blank_bgr)
                {
                    color[0] = 0xFF;
                    color[1] = 0xFF;
                    color[2] = 0xFF;
                }
            }
            img.at<Vec3b>(Point(x, y)) = color;
        }
    }
}

Mat ApplyCanny(const Mat& img)
{
    Mat out = ApplyCannyAlgoFull(img, 125, 350);
    return out;
}

Mat ApplyCannyAlgoFull(const Mat& img, int threshold, int aperture)
{
    Mat contours;
    Mat grayscale = convertograyScale(img);
    //Apply Canny algorithm
    cv::Canny(grayscale, // gray-level image
        contours, // output contours
        threshold, // low threshold
        aperture); // high threshold
    return contours;
}

// https://docs.opencv.org/3.4/d2/dbd/tutorial_distance_transform.html
Mat WaterShed(const Mat& img)
{

    Mat src = img.clone();

    // Change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform
    Mat mask;
    inRange(src, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
    src.setTo(Scalar(0, 0, 0), mask);


    // Create a kernel that we will use to sharpen our image
    Mat kernel = (Mat_<float>(3, 3) <<
        1, 1, 1,
        1, -8, 1,
        1, 1, 1);
    // an approximation of second derivative, a quite strong kernel
    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    Mat imgLaplacian;
    filter2D(src, imgLaplacian, CV_32F, kernel);
    Mat sharp;
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;
    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
    // Create binary image from source image
    Mat bw;
    cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);
    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(dist, dist, kernel1);
    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);
    // Find total markers
    std::vector<std::vector<Point> > contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32S);
    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i) + 1), -1);
    }
    // Draw the background marker
    circle(markers, Point(5, 5), 3, Scalar(255), -1);
    Mat markers8u;
    markers.convertTo(markers8u, CV_8U, 10);
    // Perform the watershed algorithm
    watershed(imgResult, markers);
    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);
    // Generate random colors
    std::vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i, j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i, j) = colors[index - 1];
            }
        }
    }
    return dst;


}

// Only worked on linux Windows 10
// Apparently there is some internal driver that prevents me from using
// cascade classifiers
// https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
std::vector<Rect> detectFacesInImage(Mat& img)
{
    if (img.type() != CV_8UC1)
    {   // not gray-level image
        convertograyScale(img);
    }

    std::vector<Rect> faces;
    CascadeClassifier cascade;
    cascade.load(CASCADE_PATH_FRONTAL);
    cascade.detectMultiScale(img, faces);
    return faces;

}

// Only worked on linux Windows 10
// Apparently there is some internal driver that prevents me from using
// cascade classifiers
// https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
std::vector<Rect> detectEyesInImage(Mat& img)
{

    if (img.type() != CV_8UC1)
    {   // not gray-level image
        convertograyScale(img);
    }

    std::vector<Rect> eyes;
    CascadeClassifier cascade;
    //cascade.load(CASCADE_PATH_FRONTAL_EYE); 
    cascade.load(CASCADE_PATH_FRONTAL_EYE_CUDA);
    cascade.detectMultiScale(img, eyes);
    return eyes;

}

/*------------------------------------------------------------------------------------------
* https://docs.opencv.org/4.x/d0/dd4/tutorial_dnn_face.html
*----------------------------------------------------------------------------------------------*/
static
void
visualize(Mat& input, int frame, Mat& faces, int thickness)
{

    for (int i = 0; i < faces.rows; i++)
    {
        // Draw bounding box
        rectangle(input, Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), Scalar(0, 255, 0), thickness);
        // Draw landmarks
        circle(input, Point2i(int(faces.at<float>(i, 4)), int(faces.at<float>(i, 5))), 2, Scalar(255, 0, 0), thickness);
        circle(input, Point2i(int(faces.at<float>(i, 6)), int(faces.at<float>(i, 7))), 2, Scalar(0, 0, 255), thickness);
        circle(input, Point2i(int(faces.at<float>(i, 8)), int(faces.at<float>(i, 9))), 2, Scalar(0, 255, 0), thickness);
        circle(input, Point2i(int(faces.at<float>(i, 10)), int(faces.at<float>(i, 11))), 2, Scalar(255, 0, 255), thickness);
        circle(input, Point2i(int(faces.at<float>(i, 12)), int(faces.at<float>(i, 13))), 2, Scalar(0, 255, 255), thickness);
    }
}

Mat detectFaces(const Mat& image)
{

    float scoreThreshold = static_cast<float>(0.9);
    float nmsThreshold = static_cast<float>(0.3);
    int topK = 5000;

    Mat empty;

    cv::Mat imgclone = convertograyScale(image);

    // this shared pointer is never initialized correctly
    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
        fd_modelPath,
        "",
        Size(320, 320),
        scoreThreshold,
        nmsThreshold,
        topK);

    if (nullptr == detector)
    {
        return empty;
    }


    Mat faces1;
    Mat out = imgclone.clone();

    detector->detect(out, faces1);
    if (faces1.rows < 1)
    {
        return empty;
    }

    visualize(out, -1, faces1);
    return out;
}
