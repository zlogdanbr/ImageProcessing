#pragma once

#include "wx/wx.h"
#include <vector>


namespace image_constants
{
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
		"Difference of Gaussians",
		"Median",
		"Sharpening",
		"Unsharp",
		"Laplacian Extended",
		"Sobel",
		"Canny Extended",
		"Hough Transform Lines",
		"Hough Transform Circles",
		"Harris Algorithm",
		"Find Contourns ( Threshold )",
		"Find Contourns ( Canny )",
		"Find Contourns Descriptors",
		"Find Sift Descriptors"
	};
}