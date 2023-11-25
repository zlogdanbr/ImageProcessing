#include "detection.h"
#include "opcvwrapper.h"


/* -------------------------------------------------------------------------------------------
	 https://docs.opencv.org/4.x/d0/dd4/tutorial_dnn_face.html

		It should work but only God knows why it is not
		I had used these functions before using Windows 10 and Linux ( Lubuntu 18 )
		I suspect that as I am running at windows 11, opencv simply does not like
		it when it runs detector->detect(out, faces1);

----------------------------------------------------------------------------------------------*/
static
void 
visualize(Mat& input, int frame, Mat& faces,  int thickness)
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
	// https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet
	std::string fd_modelPath = "C:\\Users\\Administrador\\Documents\\GitHub\\Image Data\\face_data1\\face_detection_yunet_2023mar_int8.onnx";
	// https://github.com/opencv/opencv_zoo/tree/master/models/face_recognition_sface
	std::string fr_modelPath = "C:\\Users\\Administrador\\Documents\\GitHub\\Image Data\\face_data2\\face_recognition_sface_2021dec.onnx";

	float scoreThreshold = static_cast<float>(0.9);
	float nmsThreshold = static_cast<float>(0.3);
	int topK = 5000;

	Mat empty;

	cv::Mat imgclone = convertograyScale(image);

	// this shared pointer is never initialized correctly
	Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(
		fr_modelPath,
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

	visualize(out, -1, faces1 );
	return out;
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

"As in the case with the Harris point detector, the FAST feature algorithm derives from the
definition of what constitutes a corner. This time, this definition is based on the image
intensity around a putative feature point. The decision to accept a keypoint is taken by
examining a circle of pixels centered at a candidate point. If an arc of contiguous points of a
length greater than three quarters of the circle perimeter in which all pixels significantly
differ from the intensity of the center point (being all darker or all brighter) is found, then a
keypoint is declared."
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

std::pair<std::vector<int>, std::vector<int>> getXYFromAbstractRegion(AbstractRegion& AbstractPoints)
{
	std::pair<std::vector<int>, std::vector<int>> out;
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

/**
	This function is the one I use to test algorithms I am studing
	and applying them together with other filters.
*/
Mat workingAlgorithm(const Mat& image)
{
	cv::Mat imgclone1;
	cv::Mat imgclone2;

	// Convert to gray scale

	imgclone1 = convertograyScale(image);
	imgclone2 = convertograyScale(image);


	// Apply sobel
	imgclone1 = ApplySobel(imgclone1, 5);

	// Threshold both
	Mat thr1 = ApplyThreShold(imgclone1);
	Mat thr2 = ApplyThreShold(imgclone2);

	// adjust contrast in 50% and subtract original image
	// from Sobel
	return 0.5*thr2 - thr1;
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




