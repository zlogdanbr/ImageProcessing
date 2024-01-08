#pragma once

#include "wx/wx.h"
#include <vector>


namespace image_constants
{

	/*
	*	All algorithms are identified using this vector, if you remove any of them,
	*   they will be disabled in the program
	*/
	std::vector<wxString>
		_algorithms_ =
	{
		"Undo",
		"Convert to Gray Scale",
		"Equalize Gray Scale Image",
		"Equalize Color Scale Image",
		"Flip Image Horizontally",
		"Flip Image Vertically",
		"Flip Image",
		"Blur Image",
		"Invert Image",
		"Convert to Binary",
		"Threshold",
		"Adjust Contrast",
		"Adjust Brightness",
		"Gamma Correction",
		"Crop Image",
		"Erode",
		"Dilate",
		"Closing",
		"Opening",
		"Erosion+",
		"Dilate+",
		"Segmentation Erode",
		"Morpholgical Gradient",
		"Morphological Top Hat",
		"Apply custom algo",
		"Gaussian Extended",
		"Gaussian Difference",
		"Median",
		"Laplacian Extended",
		"Sobel",
		"Canny Extended",
		"Hough Transform Lines",
		"Hough Transform Circles",
		"Find Contourns ( Threshold )",
		"Find Contourns ( Canny )",
		"Find Contourns Descriptors",
		"Find Sift Descriptors",
		"Show Sift Descriptors",
		"Create PCA file",
		"Apply Custom Kernel",
		"Find Faces"
	};
}