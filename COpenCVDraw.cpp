#include "COpenCVDraw.h"


void MyFilledCircle(Mat* img, Point& center, Scalar& s)
{
    circle( *img,
            center,
            90,
            s,
            FILLED,
            LINE_8);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    Mat* image = reinterpret_cast<Mat*>(userdata);
    if (event == EVENT_LBUTTONDOWN)
    {
        if (image->type() == CV_8UC1) 
        { 
            // gray-level image
            // single-channel 8-bit image
            Scalar s(255, 255, 255);
            Point center(x, y);
            MyFilledCircle(image, center, s);
        }
        else if (image->type() == CV_8UC3)
        { 
            // color image
            // 3-channel image
            Scalar s(255, 255, 255);
            Point center(x, y);
            MyFilledCircle(image, center, s);
        }
    }
    else if (event == EVENT_RBUTTONDOWN)
    {
        // Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        if (flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON))
        {
            //Left mouse button is clicked while pressing CTRL key - position (" << x << ", " << y << ")" << endl;
        }
    }
    else if (event == EVENT_MOUSEMOVE)
    {
        // Mouse move over the window - position (" << x << ", " << y << ")" << endl;

    }
}

COpenCVDraw::COpenCVDraw(const Mat& image)
{
	final = image.clone();
}

void COpenCVDraw::Draw()
{
    //Create a window
    namedWindow("Drawing", 1);

    //set the callback function for any mouse event
    setMouseCallback("Drawing", CallBackFunc, &final);

    //show the image
    imshow("Drawing", final);

    // Wait until user press some key
}
