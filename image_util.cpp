#include "image_util.h"

namespace image_util
{


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

    std::pair< std::vector<int>, std::vector<int>>
        getImageXY(std::vector<std::vector<Point> >& raw_contourns)
    {
        std::vector<int> x;
        std::vector<int> y;

        for (const auto& cont : raw_contourns)
        {
            for (const auto& c : cont)
            {
                x.push_back(c.x);
                y.push_back(c.y);
            }
        }

        std::pair< std::vector<int>, std::vector<int>> p(x, y);
        return p;
    }

    void drawCountourXY(std::vector<std::vector<Point> >& raw_contourns)
    {
        std::vector<int> x;
        std::vector<int> y;

        auto axes = CvPlot::makePlotAxes();
        for (const auto& cont : raw_contourns)
        {
            for (const auto& c : cont)
            {
                x.push_back(1 * c.x);
                y.push_back(-1 * c.y);
            }
        }

        axes.create<CvPlot::Series>(x, y, "-g");
        CvPlot::show("Countours", axes);
    }


    Mat image_copy(Mat& img, Range&& r1, Range&& r2)
    {
        Mat ret = img(r1, r2);
        return ret;
    }

    std::pair<double, double> getNumber(double x)
    {
        double y = 0.0;
        double d = 0.0;

        y = modf(x, &d);

        std::pair<double, double> p(d, y);

        return p;
    }

    // https://learnopencv.com/cropping-an-image-using-opencv/
    Mat cropImage(const Mat& img, int M, int N)
    {
        //int M = 76;
        //int N = 104;

        Mat clone = img.clone();

        int imgheight = img.size().height;
        int imgwidth = img.size().width;

        int x1 = 0;
        int y1 = 0;
        for (int y = 0; y < imgheight; y = y + M)
        {
            for (int x = 0; x < imgwidth; x = x + N)
            {
                if ((imgheight - y) < M || (imgwidth - x) < N)
                {
                    break;
                }
                y1 = y + M;
                x1 = x + N;

                if (x1 >= imgwidth && y1 >= imgheight)
                {
                    x = imgwidth - 1;
                    y = imgheight - 1;
                    x1 = imgwidth - 1;
                    y1 = imgheight - 1;

                    // crop the patches of size MxN
                    Mat tiles = image_copy(clone,Range(y, imgheight), Range(x, imgwidth));
                    rectangle(clone, Point(x, y), Point(x1, y1), Scalar(0, 255, 0), 1);
                }
                else if (y1 >= imgheight)
                {
                    y = imgheight - 1;
                    y1 = imgheight - 1;

                    // crop the patches of size MxN
                    Mat tiles = image_copy(clone, Range(y, imgheight), Range(x, x + N));
                    rectangle(clone, Point(x, y), Point(x1, y1), Scalar(0, 255, 0), 1);
                }
                else if (x1 >= imgwidth)
                {
                    x = imgwidth - 1;
                    x1 = imgwidth - 1;

                    // crop the patches of size MxN
                    Mat tiles = image_copy(clone, Range(y, y + M), Range(x, imgwidth));
                    rectangle(clone, Point(x, y), Point(x1, y1), Scalar(0, 255, 0), 1);
                }
                else
                {
                    // crop the patches of size MxN
                    Mat tiles = image_copy(clone, Range(y, y + M), Range(x, x + N));
                    rectangle(clone, Point(x, y), Point(x1, y1), Scalar(0, 255, 0), 1);
                }
            }

        }

        return clone;
    }

}