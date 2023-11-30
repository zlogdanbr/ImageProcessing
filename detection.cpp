#include "detection.h"
#include "opcvwrapper.h"
#include "filesys.h"


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
Mat detectCornersHarrisAlgoFull(		const Mat& image, 
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
	cv::threshold(	cornerStrength,
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
	cv::Ptr<cv::FastFeatureDetector> ptrFAST =cv::FastFeatureDetector::create(40);
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

void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE r, UBYTE g, UBYTE b, bool blank_bgr )
{
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			Vec3b color = img.at<Vec3b>(Point(x, y));

			std::pair<int, int> p{ x,y };
			auto it = std::find(abstract_region.cbegin(), abstract_region.cend(), p);

			if ( it != abstract_region.end())
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




