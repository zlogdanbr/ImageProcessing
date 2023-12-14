#include "opcvwrapper.h"
#include <wx/gdicmn.h> 

/**
    This function is the one I use to test algorithms I am studing
    and applying them together with other filters.
*/
Mat ApplyCustomAlgo(const Mat& image)
{
    Mat clone = image.clone();
    cv::Size s = clone.size();
    wxRect sizeScreen = wxGetClientDisplayRect();

    if (s.width > sizeScreen.width || s.height > sizeScreen.height)
    {
        auto rows = clone.rows;
        auto cols = clone.cols;
        Size _s;
        _s.height = rows/8;
        _s.width  = cols/8;
        resize(clone, clone, _s);
    }
    
    Rect rect = selectROI("Get ROI", clone, false);
    Mat roi = Mat(clone, rect);

    Mat saveroi = roi.clone();
    saveroi = convertograyScale(saveroi);

    waitKey(0);

    cv::Size s2 = roi.size();
    if (s2.width == 0 || s2.height == 0)
    {
        destroyWindow("Get ROI");
        return roi;
    }
    destroyWindow("Get ROI");

    roi = ApplyOpening(roi);
    //roi = ApplyCannyAlgoFull(roi, 125, 350);
    roi = InvertImage(roi);

    roi = saveroi - roi;


    return roi;
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
    cv::Size image_size = img.size();

    wxRect sizeScreen = wxGetClientDisplayRect();

    if (sizeScreen.width < image_size.width || sizeScreen.height < image_size.height)
    {
        cv::namedWindow(title, cv::WINDOW_NORMAL);
        float ratio = static_cast<float>(image_size.width) / static_cast<float>(image_size.height);
        float up_width = static_cast<float>(sizeScreen.width / 2);
        float up_height = static_cast<float>(sizeScreen.height * ratio);
        Mat resized_up;
        resize(img, resized_up, Size(up_width, up_height), INTER_LINEAR);
        imshow(title, resized_up);
    }
    else
    {
        imshow(title, img);
    }
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

Mat getBinaryImage(const Mat& img)
{
    // Create binary image from source image
    Mat bw;

    if (!isGrayScaleImage(img))
    {
        cvtColor(img, bw, COLOR_BGR2GRAY);
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

/*

src	Source 8-bit or floating-point, 1-channel or 3-channel image.
dst	Destination image of the same size and type as src .
d	Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.

sigmaColor	Filter sigma in the color space. A larger value of the parameter means that farther colors within
the pixel neighborhood (see sigmaSpace) will be mixed together, resulting in larger areas of semi-equal color.

sigmaSpace	Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will 
influence each other as long as their colors are close enough (see sigmaColor ). When d>0, it specifies the 
neighborhood size regardless of sigmaSpace. Otherwise, d is proportional to sigmaSpace.
borderType	border mode used to extrapolate pixels outside of the image, see BorderTypes

*/

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

// https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
Mat ApplyHoughTransformCustom(const Mat& img)
{
    Mat dst;
    Mat cdst;

    // Apply Canny algorithm
    cv::Mat contours;
    Canny(img, dst, 50, 200, 3);

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

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

Mat ApplyErode(const Mat& img)
{
    cv::Mat eroded; // the destination image
    cv::erode(img, eroded, cv::Mat());
    return eroded;
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


/* -------------------------------------------------------------------------------------------
OpenCV 3 Computer Vision
Application Programming
Cookbook
Third Edition
Robert Laganiere
Page [ 239 ]
See page [245 ] for the math
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

