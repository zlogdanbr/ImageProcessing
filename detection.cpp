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

Mat detectEyes(const Mat& image)
{
	// https://github.com/opencv/opencv_zoo/tree/master/models/face_detection_yunet
	std::string fd_modelPath = "C:\\Users\\Administrador\\Documents\\GitHub\\Image Data\\face_data1\\face_detection_yunet_2023mar_int8.onnx";
	// https://github.com/opencv/opencv_zoo/tree/master/models/face_recognition_sface
	std::string fr_modelPath = "C:\\Users\\Administrador\\Documents\\GitHub\\Image Data\\face_data2\\face_recognition_sface_2021dec.onnx";

	float scoreThreshold = 0.9;
	float nmsThreshold = 0.3;
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
Mat detectCorners(const Mat& image)
{

	// Detect Harris Corners
	cv::Mat cornerStrength;

	// the book does not say but you need to convert to a gray scale image
	cv::Mat imgclone = convertograyScale(image);

	cv::cornerHarris(	imgclone, // input image
						cornerStrength, // image of cornerness
						3, // neighborhood size
						3, // aperture size
						0.01); // Harris parameter


	// threshold the corner strengths
	cv::Mat harrisCorners;
	double threshold = 0.0001;
	cv::threshold(cornerStrength,
		harrisCorners,
		threshold,
		255,
		cv::THRESH_BINARY);
	return harrisCorners;
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
Mat detect(const Mat& image)
{
	// the book does not say but you need to convert to a gray scale image
	cv::Mat imgclone = convertograyScale(image);

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

Mat detect2(const Mat& image)
{
	// the book does not say but you need to convert to a gray scale image
	cv::Mat imgclone = image.clone();

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

Mat custom_algo(const Mat& image)
{

	Mat img2 = convertograyScale(image);
	showImage(image, "Original Image");

	img2 = blurImageSmooth(img2, 3);
	if (img2.empty() == false)
	{
		showImage(img2, "Image blurrer 3x3 Kernel");
	}

	img2 = blurImageSmooth(img2, 5);
	if (img2.empty() == false)
	{
		showImage(img2, "Image blurrer 5x5 Kernel");
	}

	img2 = GaussianImageSmooth(img2, 5);
	if (img2.empty() == false)
	{
		showImage(img2, "Gaussian 5x5 Kernel");
	}

	img2 = detect2(img2);
	if (img2.empty() == false)
	{
		showImage(img2, "Detected");
	}
	return img2;
}


