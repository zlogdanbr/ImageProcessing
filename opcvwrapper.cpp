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
