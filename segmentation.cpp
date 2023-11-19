#include "segmentation.h"
#include "detection.h"

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


