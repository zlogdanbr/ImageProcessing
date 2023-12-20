#pragma once

#include "opencv2/objdetect.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

class COpenCVDraw
{
public:
	COpenCVDraw(const Mat& image);
	Mat getfinal() { return final; };
	void Draw();
private:
	Mat final;
};

