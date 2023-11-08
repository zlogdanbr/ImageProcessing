#include "segmentation.h"


bool NoduleRec::findContornos(int threshold)
{
	original.copyTo(final);
	findcontours(final, this->contours, this->hierarchy, threshold, this->edges);
	return final.empty();
}

void NoduleRec::HighlightRoi()
{
	//final.release();
	original.copyTo(final);
	drawCountour(this->contours, final, this->hierarchy);
}

std::vector<Rect> EyesDetector::removeNonEyes(std::vector<Rect>& eyes, int tol)
{
	std::vector<Rect> ActualEyes;

	for (auto& it : eyes)
	{
		if  ( calculateRadius(it) >= tol )
		{
			ActualEyes.push_back(it);
		}
	}

	return ActualEyes;
}


Mat EyesDetector::findEyes(int tol, int option)
{
	Mat eyes_img;
	if (option == 1)
		eyes_img = n.getFinalImg();
	else
		eyes_img = n.getOriginalImg();

	std::vector<Rect> eyes = detectEyesInImage(eyes_img);
	std::vector<Rect> ActualEyes = removeNonEyes(eyes, tol);

	for (auto& r : ActualEyes)
	{
		drawCirclesAtImgFromRoi(eyes_img, r);
	}

	return eyes_img;
}


