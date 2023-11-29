#include "wxwimage_algos.h"
#include "wx/image.h"

wxImage rotate_left(wxImage& image)
{
	wxImage image2 = image.Rotate90(false);
	return image2;
}

wxImage rotate_right(wxImage& image)
{
	wxImage image2 = image.Rotate90(true);
	return image2;
}

wxImage scalePlusImage(wxImage& image, int scale, Dimensions& d)
{
    auto _h = image.GetHeight();
    auto _w = image.GetWidth();
    auto hn = static_cast<int>(_h * scale);
    auto wn = static_cast<int>(_w * scale);

    d.first = hn;
    d.second = wn;

    wxImage image2 = image.Scale(hn, wn);
    return image2;
}

wxImage scaleLessImage(wxImage& image, int scale, Dimensions& d)
{
    auto _h = image.GetHeight();
    auto _w = image.GetWidth();
    auto hn = static_cast<int>(_h / scale);
    auto wn = static_cast<int>(_w / scale);

    if (hn <= 0)
    {
        hn = _h;
    }

    if (wn <= 0)
    {
        wn = _w;
    }

    d.first = hn;
    d.second = wn;

    wxImage image2 = image.Scale(hn, wn);
    return image2;
}

wxImage Blur(wxImage& image, int blurRadius)
{
    wxImage image2 = image.BlurVertical(blurRadius);
    return image2;
}

wxImage BlurH(wxImage& image, int blurRadius)
{
    wxImage image2 = image.BlurHorizontal(blurRadius);
    return image2;
}

wxImage BlurV(wxImage& image, int blurRadius)
{
    wxImage image2 = image.Blur(blurRadius);
    return image2;
}

wxImage ConvertToGrayScale(wxImage& image)
{
    wxImage image2 = image.ConvertToGreyscale();
    return image2;
}
