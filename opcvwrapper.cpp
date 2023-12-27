#include "opcvwrapper.h"
#include "image_util.h"
#include <iostream>
#include <fstream>
#include <wx/gdicmn.h> 
#include <wx/textdlg.h>
#include "image_interest_points.h"



void ApplyAndCompare(std::vector<Mat>& images)
{
    //Descriptors& descriptors
    std::vector< Descriptors > descriptors_set;
    Size Standard = images[0].size();
    for (const auto& i : images)
    {
        if (i.size().width > Standard.width && i.size().height > Standard.height)
        {
            Standard = i.size();
        }
    }

    int standard_size_width = Standard.width;
    int standard_size_height = Standard.height;

    std::vector<wxString> choices = {   
                                        "Hough Lines",
                                        "Hough Circles",
                                        "Find Contourns",
                                        "Morphological Gradient",
                                        "Morphological Top Hat",
                                        "Difference of Gaussians",
                                        "Morphological Segmentation"                                                                                  
                                    };
    wxSingleChoiceDialog dialog(
        NULL,
        "Choose Algorithm",
        "Choose Algorithm",
        static_cast<int>(choices.size()), choices.data());

    dialog.ShowModal();

    image_util::Function1Parameter option = ApplyFindContournsThreshold;

    wxString algo = dialog.GetStringSelection();
    if (algo == "Hough Lines")
    {
        option = ApplyHoughTransformLines;
    }
    else
    if (algo == "Hough Circles")
    {
        option = ApplyHoughTransformCircles;
    }
    else
    if (algo == "Find Contourns")
    {
        option = ApplyFindContournsCanny;
    }
    else
    if (algo == "Morphological Gradient")
    {
        option = ApplyMorphGradient;
    }
    else
    if (algo == "Morphological Top Hat")
    {
        option = ApplyTopHatAlgo;
    }
    else
    if (algo == "Difference of Gaussians")
    {
        option = ApplyDIfferenceOfGaussian;
    }
    else
    if (algo == "Morphological Segmentation")
    {
        option = segmentErode;
    }

    for (int i = 0; i < images.size(); i++)
    {
        if (i != 0)
        {
            resize(images[i], images[i], Size(standard_size_width, standard_size_height), INTER_AREA);
        }

        images[i] = option(images[i]);
    }

    image_util::showManyImagesOnScreen(images);


}
/**
    This function is the one I use to test algorithms I am studing
    and applying them together with other filters.
*/
Mat ApplyCustomAlgo(const Mat& image)
{
    Mat img1 = convertograyScale(image);
    Mat img2 = img1.clone();

    img1 = ApplyErode(img1);


    return img2 - img1;

}

Mat ApplyDIfferenceOfGaussian(const Mat& im)
{
    Mat img1 = GaussianImageSmoothExtended(im, 3, 0.1, 0.1);
    Mat img2 = GaussianImageSmoothExtended(im, 3, 1, 1);
    img1 = ApplyLaplacianExtended(img1);
    img2 = ApplyLaplacianExtended(img2);

    Mat final = img1 - img2;

    return final;
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
    if (img.type() == CV_8UC1)
    { 
        // gray-level image
        // single-channel 8-bit image
        return true;
    }
    else if (img.type() == CV_8UC3) 
    { 
        // color image
        return false;
    }
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
    using namespace image_util;
    cv::Size image_size = img.size();

    wxRect sizeScreen = wxGetClientDisplayRect();
    Mat clone = img.clone();
    fitImageOnScreen(clone, sizeScreen.width, sizeScreen.height);
    auto axes = CvPlot::plotImage(clone);
    cv::Mat mat = axes.render(clone.size().width, clone.size().height);
    CvPlot::show(title, axes);
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

Mat adjustContrast(const Mat& img, int factor)
{
    if (factor <= 0 || factor > 100)
    {
        factor = 50;
    }
    double toFactor = (static_cast<double>(factor) / static_cast<double>(100));
    Mat final = img.clone();
    final = final * toFactor;
    return final;
}

Mat adjustBrightness(const Mat& img, int factor)
{
    Mat final = img.clone();
    final = final + factor;
    return final;
}

Mat adjustGama(const Mat& img, double gamma)
{
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
    {
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
        
    Mat res = img.clone();
    LUT(img, lookUpTable, res);
    return res;
}

Mat getBinaryImage(const Mat& img)
{
    // Create binary image from source image
    Mat bw;

    if (!isGrayScaleImage(img))
    {
        try
        {
            cvtColor(img, bw, COLOR_BGR2GRAY);
        }
        catch (...)
        {
            bw = img.clone();
        }        
    }
    else
    {
        bw = img.clone();
    }
    
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

Mat ApplyBilateralFilterExt(const Mat& img, int kernel_size, double sigma1, double sigma2)
{
    Mat Gray = convertograyScale(img);
    Mat Blurred;

    bilateralFilter(Gray, Blurred, -1, sigma1, sigma2);

    return Blurred;
}

Mat GaussianImageSmoothExtended(    const Mat& img, 
                                    int kernel_size,
                                    double sigmaX,
                                    double sigmaY
                                )
{
    Mat Gray = convertograyScale(img);
    Mat Blurred;
    GaussianBlur(   
                    Gray, 
                    Blurred, 
                    Size(kernel_size, kernel_size), 
                    sigmaX, // sigmaX standard deviation in X direction
                    0 // sigmaY standard deviation in Y directiom 
                       //if sigmaY is zero, it is set to be equal to sigma                    
                );
    return Blurred;
}

Mat ApplyThreShold(const Mat& img, double _threshold)
{
    cv::Mat out;
    cv::threshold(img,
        out,
        _threshold,
        255,
        cv::THRESH_BINARY);
    return out;
}

// https://docs.opencv.org/3.4/d5/db5/tutorial_laplace_operator.html
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

    Mat src_gray = convertograyScale(img);

    //  src: Source image
    //  dst : Destination image
    //  ddepth : The depth of dst.A negative value(such as −1) indicates that the depth is the same as the source.
    //  kernel : The kernel to be scanned through the image
    //  anchor : The position of the anchor relative to its kernel.The location Point(-1, -1) indicates the center by default.
    //  delta : A value to be added to each pixel during the correlation.By default it is 0
    //  BORDER_DEFAULT : We let this value by default (more details in the following tutorial)

    // image,result,image.depth(),kernel
    filter2D(   src_gray,
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
Mat ApplySobelXExtended(    const Mat& img,
                    int image_type, 
                    int depth, 
                    int type,
                    double delta,
                    int kernel_size)
{
    Mat sobelX;
    Mat src_gray = convertograyScale(img);

    cv::Sobel(src_gray,            // input image
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

Mat ApplySobelYExtended(const Mat& img,
                        int image_type,
                        int depth,
                        int type,
                        double delta,
                        int kernel_size)
{
    Mat sobelY;
    Mat src_gray = convertograyScale(img);

    cv::Sobel(src_gray,      // input image
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

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
My note: for further processing the std::vector<Vec4i> linesP is what we will use in later
processing
----------------------------------------------------------------------------------------------*/
Mat ApplyHoughTransformLines(const Mat& img)
{
    Mat dst;
    Mat cdst;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    // Probabilistic Line Transform
    std::vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(    dst, 
                    linesP, 
                    1,              
                    CV_PI / 180, 
                    50, 
                    50, 
                    10); // runs the actual detection

    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++)
    {
        Vec4i l = linesP[i];
        line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }

    return cdst;
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
My note: for further processing the std::vector<cv::Vec3f> circles is what we will use in later
processing
----------------------------------------------------------------------------------------------*/
Mat ApplyHoughTransformCircles(const Mat& img)
{
    Mat dst;
    Mat cdst;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    // Standard Hough circles Transform
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(   dst, 
                        circles, 
                        cv::HOUGH_GRADIENT,
                        2, //accumulator resolution (size of the image/2)
                        50, // minimum distance between two circles
                        200, // Canny high threshold
                        100, // minimum number of votes
                        25,
                        100); // min and max radius

    auto itc = circles.begin();
    while ( itc != circles.end()) 
    {
        cv::circle( cdst,
                    cv::Point((*itc)[0], (*itc)[1]), // circle centre
                    (*itc)[2], // circle radius
                    cv::Scalar(255), // color
                    2); // thickness
        ++itc;
    }
    return cdst;

}


Mat Sharpening(const Mat& img)
{
    Mat Iconv = 0.5 * ApplyLaplacianExtended(img, 3, 1);
    Mat igray = convertograyScale(img);
    return igray - Iconv;
}

Mat Unsharp(const Mat& img)
{
    Mat Iconv = GaussianImageSmoothExtended(img,3, 150,150);
    Mat igray = convertograyScale(img);
    return (1.5) * igray - 0.5 * Iconv;
}

Mat ApplyErodeEx(const Mat& img,int type)
{
    cv::Mat eroded; // the destination image
    cv::erode(img, eroded, type);
    return eroded;
}

Mat ApplyDilateEx(const Mat& img,int type)
{
    cv::Mat dilated; // the destination image
    cv::dilate(img, dilated, type);
    return dilated;
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

Mat ApplyClosing(const Mat& img)
{
    cv::Mat final;
    final = ApplyErode(img);
    final = ApplyDilate(final);
    return final;
}

Mat ApplyOpening(const Mat& img)
{
    cv::Mat final;
    final = ApplyDilate(img);
    final = ApplyErode(final);    
    return final;
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

Mat segmentErode(const Mat& img)
{
    Mat img1 = convertograyScale(img);
    Mat img2 = img1.clone();

    img1 = ApplyErode(img1);


    return img2 - img1;
}

/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 239 ]
Page [245 ] for the math
----------------------------------------------------------------------------------------------*/
Mat detectCornersHarrisAlgoFull(    const Mat& image,
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

    cv::cornerHarris(   imgclone, // input image
                        cornerStrength, // image of cornerness
                        2, // neighborhood size
                        3, // aperture size
                        0.04); // Harris parameter

    cv::Mat harrisCorners;

    // threshold the corner strengths
    cv::threshold(  cornerStrength,
                    harrisCorners,
                    threshold,
                    255,
                    cv::THRESH_BINARY);

    return harrisCorners;
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

std::vector<Vec4i> GetLinesHoughTransform(  const Mat& img,
                                            double rho,
                                            double theta,
                                            double minLineLength,
                                            double maxLineGap)
{
    Mat dst;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Probabilistic Line Transform
    std::vector<Vec4i> linesP; // will hold the results of the detection

    /*----------------------------------------------------------------------------------------------------------------
        image	        8-bit, single-channel binary source image. The image may be modified by the function.
        lines	        Output vector of lines. Each line is represented by a 4-element vector (x1,y1,x2,y2) ,
                        where (x1,y1) and (x2,y2) are the ending points of each detected line segment.
        rho	            Distance resolution of the accumulator in pixels.
        theta	        Angle resolution of the accumulator in radians.
        threshold	    Accumulator threshold parameter. Only those lines are returned that get enough votes ( >threshold ).
        minLineLength	Minimum line length. Line segments shorter than that are rejected.
        maxLineGap	    Maximum allowed gap between points on the same line to link them.

    ----------------------------------------------------------------------------------------------------------------*/
    HoughLinesP(dst,
        linesP,
        rho,
        theta,
        50,
        minLineLength,
        maxLineGap);

    return linesP;
}

std::vector<cv::Vec3f> GetCirclesHoughTransform(const Mat& img,
                                                int method,
                                                double dp,
                                                double minDist,
                                                double param1,
                                                double param2,
                                                int  minRadius,
                                                int  maxRadius)
{
    Mat dst;
    Mat cdst;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

    // Standard Hough circles Transform
    std::vector<cv::Vec3f> circles;

    /*------------------------------------------------------------------------------------------------
        image	    8-bit, single-channel, grayscale input image.
        circles	    Output vector of found circles. Each vector is encoded as 3 or 4
                    element floating-point vector (x,y,radius) or (x,y,radius,votes) .
        method	    Detection method, see HoughModes.
                    The available methods are HOUGH_GRADIENT and HOUGH_GRADIENT_ALT.
        dp	        Inverse ratio of the accumulator resolution to the image resolution.
                    For example, if dp=1 , the accumulator has the same resolution as the input image.
                    If dp=2 , the accumulator has half as big width and height.
                    For HOUGH_GRADIENT_ALT the recommended value is dp=1.5, unless some
                    small very circles need to be detected.
        minDist	    Minimum distance between the centers of the detected circles.
                    If the parameter is too small, multiple neighbor circles may be falsely detected
                    in addition to a true one. If it is too large, some circles may be missed.
        param1	    First method-specific parameter. In case of HOUGH_GRADIENT and HOUGH_GRADIENT_ALT,
                    it is the higher threshold of the two passed to the Canny edge detector (the lower one is twice smaller).
                    Note that HOUGH_GRADIENT_ALT uses Scharr algorithm to compute image derivatives,
                    so the threshold value shough normally be higher, such as 300 or normally exposed and contrasty images.
        param2	    Second method-specific parameter. In case of HOUGH_GRADIENT, it is the
                    accumulator threshold for the circle centers at the detection stage.
                    The smaller it is, the more false circles may be detected. Circles,
                    corresponding to the larger accumulator values, will be returned first.
                    In the case of HOUGH_GRADIENT_ALT algorithm, this is the circle "perfectness" measure.
                    The closer it to 1, the better shaped circles algorithm selects.
                    In most cases 0.9 should be fine. If you want get better detection of small circles,
                    you may decrease it to 0.85, 0.8 or even less. But then also try to limit the search range
                    [minRadius, maxRadius] to avoid many false circles.
        minRadius	Minimum circle radius.
        maxRadius	Maximum circle radius. If <= 0, uses the maximum image dimension.
                    If < 0, HOUGH_GRADIENT returns centers without finding the radius.
                    HOUGH_GRADIENT_ALT always computes circle radiuses.
    ----------------------------------------------------------------------------------------------------*/
    cv::HoughCircles(dst,
        circles,
        cv::HOUGH_GRADIENT,
        dp, //accumulator resolution (size of the image/2)
        minDist, // minimum distance between two circles
        param1, // 
        param2, // 
        minRadius,
        maxRadius); // min and max radius

    return circles;

}

Mat ApplyFindContournsThreshold(const Mat& img)
{
    Mat src_gray;
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

    Mat drawing = Mat::zeros(bw.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(drawing, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
    }

    return drawing;
}

Mat ApplyFindContournsCanny(const Mat& img)
{
    Mat src_gray;
    int thresh = 50;
    RNG rng(12345);

    Mat src = img.clone();

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    Mat canny_output;
    Canny(src_gray, canny_output, thresh, 250);

    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(drawing, contours, static_cast<int>(i), Scalar(0, 0, 255), 2);
    }

    return drawing;
}

/*
I have just copied and pasted and edited here so I could see this working
it is not active at all unless you run this code using the function
ApplyCustomAlgo, replacing the body function with

MM(img);

// https://learnopencv.com/deep-learning-with-opencvs-dnn-module-a-definitive-guide/

*/
Mat NN(const Mat& img)
{
    const std::string NNFolder = "C:\\Users\\Administrador\\Documents\\GitHub\\Image Data\\NN";
    Mat image = img.clone();
    std::vector<std::string> class_names;
    std::ifstream ifs(std::string(NNFolder + "\\" + "object_detection_classes_coco.txt").c_str());
    std::string line;
    while (std::getline(ifs, line))
    {
        class_names.push_back(line);
    }

    // load the neural network model
    auto model = readNet(   NNFolder+"\\"+"frozen_inference_graph.pb", 
                            NNFolder + "\\" + "ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt", 
                            "TensorFlow");

    // read the image from disk

    int image_height = image.cols;
    int image_width = image.rows;
    //create blob from image
    Mat blob = blobFromImage(image, 1.0, Size(300, 300), Scalar(127.5, 127.5, 127.5), true, false);
    //create blob from image
    model.setInput(blob);
    //forward pass through the model to carry out the detection
    Mat output = model.forward();
    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) 
    {
        int class_id = detectionMat.at<float>(i, 1);
        float confidence = detectionMat.at<float>(i, 2);

        // Check if the detection is of good quality
        if (confidence > 0.4) {
            int box_x = static_cast<int>(detectionMat.at<float>(i, 3) * image.cols);
            int box_y = static_cast<int>(detectionMat.at<float>(i, 4) * image.rows);
            int box_width = static_cast<int>(detectionMat.at<float>(i, 5) * image.cols - box_x);
            int box_height = static_cast<int>(detectionMat.at<float>(i, 6) * image.rows - box_y);
            rectangle(image, Point(box_x, box_y), Point(box_x + box_width, box_y + box_height), Scalar(255, 255, 255), 2);
            putText(image, class_names[class_id - 1].c_str(), Point(box_x, box_y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(57, 89, 255), 1);
        }
    }
    return image;
}