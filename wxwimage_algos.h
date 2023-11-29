#pragma once

#include "childframes.h"

using Dimensions = std::pair<int, int>;

wxImage rotate_left(wxImage& image);
wxImage rotate_right(wxImage& image);
wxImage scalePlusImage(wxImage& image, int scale, Dimensions&);
wxImage scaleLessImage(wxImage& image, int scale, Dimensions&);


wxImage Blur(wxImage& image, int blurRadius);
wxImage BlurH(wxImage& image, int blurRadius);
wxImage BlurV(wxImage& image, int blurRadius);

wxImage ConvertToGrayScale(wxImage& image);

