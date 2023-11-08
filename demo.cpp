#include "demo.h"

bool FindAndDrawCircles(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return false;
    }
    std::vector<Rect> faces = detectFacesInImage(img);

    for (auto& roi : faces)
    {
        drawCirclesAtImgFromRoi(img, roi);
    }

    showImage(img, "Final");

    return true;
    
}

void FindAndDrawRectangles(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }
    std::vector<Rect> faces = detectFacesInImage(img);

    for (auto& roi : faces)
    {
        drawSquaresAtImgFromRoi(img, roi);
    }
    showImage(img, "Final");
}

void FindAndDFacesAndEyes(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }
    std::vector<Rect> faces = detectFacesInImage(img);

    for (auto& roi : faces)
    {
        drawCirclesAtImgFromRoi(img, roi);
        Mat faceROI = convertRectoImg(roi, img);
        std::vector<Rect> eyes = detectEyesInImage(faceROI);
        for (auto& roieye : eyes)
        {
            drawCirclesAtImgFromRoi(faceROI, roieye);
        }
    }


    showImage(img, "Final");
}

void FindAndDFacesAndEyes(Mat& img)
{
    std::vector<Rect> faces = detectFacesInImage(img);

    for (auto& roi : faces)
    {
        drawCirclesAtImgFromRoi(img, roi);
        Mat faceROI = convertRectoImg(roi, img);
        std::vector<Rect> eyes = detectEyesInImage(faceROI);
        for (auto& roieye : eyes)
        {
            drawCirclesAtImgFromRoi(faceROI, roieye);
        }
    }


    showImage(img, "Final");
}

void segmentImage(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }
    std::vector<Rect> faces = detectFacesInImage(img);
    Mat final = img;
    for (auto roi = faces.begin(); roi != faces.end(); roi++)
    {
        segmentationOfROI(final, *roi, 255,255,255);
    }
    showImage(final, "final");
}

/**
* ImageAdjustmentsTesting
* it tests adjusments in a photo specified by the path image_path
*/
void ImageAdjustmentsTesting(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }

    Mat img2 = convertograyScale(img);
    showImage(img, "Original Image");
    if (img2.empty() == false)
    {
        showImage(img2, "Gray Scale");
        Mat img3 = equalizeGrayImage(img2);

        if (img3.empty() == false)
        {
            showImage(img3, "Gray Scale Histogram Adjusted");
        }

        Mat img4 = equalizeColorImage(img);
        if (img4.empty() == false)
        {
            showImage(img4, "Color Image Histogram Adjusted");
        }
    }

    Mat img5 = blurImageSmooth(img, 3);
    if (img5.empty() == false)
    {
        showImage(img5, "Image blurrer 3x3 Kernel");
    }

    Mat img6 = blurImageSmooth(img, 5);
    if (img6.empty() == false)
    {
        showImage(img6, "Image blurrer 5x5 Kernel");
    }

    Mat img7 = GaussianImageSmooth(img, 5);
    if (img7.empty() == false)
    {
        showImage(img7, "Gaussian 5x5 Kernel");
    }
}

/**
* detecting
* Finds edges in a poto specified by the path image_path using the laplacian
* algorithm.
*/
void FindEdgesOnPhoto(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }

    //showImage(img, "Original" );
    //showImage(imglp, "Laplacian");

    Mat Imfinal = convertograyScale(img) - laplacian(img);

    RoiAretype contours;
    std::vector<Vec4i> hierarchy;
    int threshold = 10;
    std::cout << "Type the threshold level: ";
    std::cin >> threshold;
    
    Mat edges;
    findcontours(Imfinal, contours, hierarchy, threshold,edges);
    drawCountour(contours, Imfinal, hierarchy);
    showImage(Imfinal, "Contornos");
}

void ApplyCustomFilter(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }
    Point p{ -1,-1 };
    // 
    Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0,
        -1, 5, -1,
        0, -1, 0);

    Mat final = ApplyCustom2Dfilter(img, kernel, -1, p, 0);
    showImage(final, "2D Filter");
}

void FindTheCountours(const std::string& image_path)
{
    Mat img;

    if (loadImage(image_path, img) == false)
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }

    RoiAretype contours;
    std::vector<Vec4i> hierarchy;
    int threshold = 10;
    std::cout << "Type the threshold level: ";
    std::cin >> threshold;

    Mat edges;
    findcontours(img, contours, hierarchy, threshold,edges);
    drawCountour(contours, img, hierarchy);
    showImage(img, "Contornos");


}


