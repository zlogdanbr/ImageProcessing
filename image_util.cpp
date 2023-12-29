#include "image_util.h"

namespace image_util
{
    // https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
    void drawCirclesAtImgFromRoi(Mat& img, Rect& roi)
    {
        Point Mycenter(roi.x + roi.width / 2,
            roi.y + roi.height / 2);

        int radius = cvRound((roi.width + roi.height) * 0.25);
        circle(img, Mycenter, radius, Scalar(255, 0, 0), 4);
    }

    // https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
    void drawSquaresAtImgFromRoi(Mat& img, Rect& roi)
    {
        Scalar color = Scalar(255, 0, 0);
        rectangle(img,
            Point(cvRound(roi.x * 1),
                cvRound(roi.y * 1)),
            Point(cvRound((roi.x + roi.width - 1) * 1),
                cvRound((roi.y + roi.height - 1) * 1)),
            color,
            3,
            8,
            0);
    }

    //https://docs.opencv.org/4.x/d4/d1b/tutorial_histogram_equalization.html
    std::vector<Mat> splitChannel(Mat& img)
    {
        Mat colorrgb;
        cvtColor(img, colorrgb, COLOR_BGR2YCrCb);
        std::vector<Mat> vec_channels;
        split(colorrgb, vec_channels);
        return vec_channels;
    }

    // https://docs.opencv.org/4.x/d5/d98/tutorial_mat_operations.html
    Mat convertRectoImg(Rect& r, Mat& img)
    {
        Mat roi = Mat(img, r);
        return roi;
    }

    // http://cool-emerald.blogspot.com/2017/11/opencv-with-wxwidgets.html
    wxImage wx_from_mat(Mat& img)
    {
        Mat im2;
        if (img.channels() == 1) { cvtColor(img, im2, COLOR_GRAY2RGB); }
        else if (img.channels() == 4) { cvtColor(img, im2, COLOR_BGRA2RGB); }
        else { cvtColor(img, im2, COLOR_BGR2RGB); }
        long imsize = im2.rows * im2.cols * im2.channels();
        wxImage wx(im2.cols, im2.rows, (unsigned char*)malloc(imsize), false);
        unsigned char* s = im2.data;
        unsigned char* d = wx.GetData();
        for (long i = 0; i < imsize; i++) { d[i] = s[i]; }
        return wx;
    }

    // http://cool-emerald.blogspot.com/2017/11/opencv-with-wxwidgets.html
    Mat mat_from_wx(wxImage& wx)
    {
        Mat im2(Size(wx.GetWidth(), wx.GetHeight()), CV_8UC3, wx.GetData());
        cvtColor(im2, im2, COLOR_RGB2BGR);
        return im2;
    }

    void putpixel(int i, int j, wxImage img, RGB& rgb)
    {
        img.SetRGB(i, j, rgb[0], rgb[1], rgb[2]);
    }

    void drawcircle(int x0, int y0, int radius, wxImage img, RGB& r)
    {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y)
        {
            putpixel(x0 + x, y0 + y, img, r);
            putpixel(x0 + y, y0 + x, img, r);
            putpixel(x0 - y, y0 + x, img, r);
            putpixel(x0 - x, y0 + y, img, r);
            putpixel(x0 - x, y0 - y, img, r);
            putpixel(x0 - y, y0 - x, img, r);
            putpixel(x0 + y, y0 - x, img, r);
            putpixel(x0 + x, y0 - y, img, r);

            if (err <= 0)
            {
                y += 1;
                err += 2 * y + 1;
            }

            if (err > 0)
            {
                x -= 1;
                err -= 2 * x + 1;
            }
        }
    }

    Mat fitImageOnScreen(Mat& img, int wscreen, int hscreen)
    {
        Mat out;
        int h = img.size().height;
        int w = img.size().width;       
        double area_screen = wscreen * hscreen;
        double area_img = h*w;
        float ratio = area_screen / area_img;

        if (ratio < 1)
        {
            int w1 = w / 2;
            int h1 = (area_screen + area_img) / w;                
            Size s(w1, h1);
            resize(img, out, s);
        }
        else
        {
            out = img.clone();
        }
        
        return out;
    }

    void showManyImagesOnScreen(std::vector<Mat>& images)
    {
        int number_of_images = images.size();
        wxRect sizeScreen = wxGetClientDisplayRect();
        double area_screen = sizeScreen.width * sizeScreen.height;
        double area_img = images[0].size().width * images[0].size().height;

        double ratio = area_screen / area_img;

        if (ratio < 1)
        {
            for (auto& i : images)
            {
                Size s(i.size().width / 2, i.size().height / 2);
                resize(i, i, s);
            }
        }

        area_img = images[0].size().width * images[0].size().height;

        ratio = area_screen / area_img;

        int cnt = 1;
        for (const auto& i : images)
        {
            imshow(std::to_string(cnt), i);
            cnt++;
        }

    }

}