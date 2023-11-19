#include "opcvwrapper.h"
#include <mutex> 

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

Mat SumImages(const Mat& img1, const Mat& img2)
{ 
    auto src_gray1 = convertograyScale(img1);
    //auto src_gray2 = convertograyScale(img2);
    Mat f;
    try
    {
        f = src_gray1 + img2;
    }
    catch (...)
    {
        return f;
    }
    
    return f;
}

Mat SubImages(const Mat& img1, const Mat& img2)
{
    auto src_gray1 = convertograyScale(img1);
    //auto src_gray2 = convertograyScale(img2);
    Mat f;
    try
    {
        f = src_gray1 - img2;
    }
    catch (...)
    {
        return f;
    }

    return f;
}

Mat XorImages(const Mat& img1, const Mat& img2)
{
    auto src_gray1 = convertograyScale(img1);
    //auto src_gray2 = convertograyScale(img2);
    Mat f;
    try
    {
        f = src_gray1 ^ img2;
    }
    catch (...)
    {
        return f;
    }

    return f;
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
    Mat grayscaleimage;
    //change the color image to grayscale image
    cvtColor(img, grayscaleimage, COLOR_BGR2GRAY);
    return grayscaleimage;
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

Mat ApplyThreShold(const Mat& img, double threshold)
{
    cv::Mat out;
    cv::threshold(img,
        out,
        threshold,
        255,
        cv::THRESH_BINARY);
    return out;
}


// https://docs.opencv.org/3.4/d5/db5/tutorial_laplace_operator.html
Mat laplacian(const Mat& src)
{
    // Declare the variables we are going to use
    Mat src_gray, dst;
    int kernel_size = 9;
    int scale = 1;
    int delta = -1;
    int ddepth = CV_16S;

    // Reduce noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    src_gray = convertograyScale(src);
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


// https://docs.opencv.org/4.x/d4/dbd/tutorial_filter_2d.html
Mat ApplyCustom2Dfilter(const Mat& img, Mat& kernel)
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
