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

    TargetPoints getXYFromAbstractRegion(AbstractRegion& AbstractPoints)
    {
        TargetPoints out;
        std::vector<int> x;
        std::vector<int> y;

        for (const auto& point : AbstractPoints)
        {
            x.push_back(point.first);
            y.push_back(point.second);
        }

        out = make_pair(x, y);

        return out;

    }

    AbstractRegion convertKeyPointsToAbstract(std::vector<cv::KeyPoint>& keypoints)
    {
        AbstractRegion  AbstractPoints;
        for (const auto& p : keypoints)
        {
            int ix = static_cast<int>(p.pt.x);
            int iy = static_cast<int>(p.pt.y);
            std::pair<int, int> pr{ ix,iy };
            AbstractPoints.push_back(pr);
        }
        return AbstractPoints;
    }

    void highlightFeature(Mat& img, AbstractRegion& abstract_region, UBYTE r, UBYTE g, UBYTE b, bool blank_bgr)
    {
        for (int y = 0; y < img.rows; y++)
        {
            for (int x = 0; x < img.cols; x++)
            {
                Vec3b color = img.at<Vec3b>(Point(x, y));

                std::pair<int, int> p{ x,y };
                auto it = std::find(abstract_region.cbegin(), abstract_region.cend(), p);

                if (it != abstract_region.end())
                {
                    color[0] = r;
                    color[1] = g;
                    color[2] = b;
                }
                else
                {
                    if (blank_bgr)
                    {
                        color[0] = 0xFF;
                        color[1] = 0xFF;
                        color[2] = 0xFF;
                    }
                }
                img.at<Vec3b>(Point(x, y)) = color;
            }
        }
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

}