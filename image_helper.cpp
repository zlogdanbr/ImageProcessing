#include "image_helper.h"

std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}


bool CImageHelper::SaveImage(wxString& Path)
{

    if (getFinalGray() == false)
    {
        wxImage ImgToSave;
        if (convertOpenCVMatToWxImage(Final_ImageOpenCVFormat, ImgToSave))
        {
            return ImgToSave.SaveFile(Path);
        }
        return false;
    }
    else
    {
        std::string spath = convertWxStringToString(Path);
        Mat final = getFinalImageOpenCV();
        if (saveImage(spath, final))
        {
            return true;
        }
        return false;
    }

}

bool CImageHelper::convertOpenCVMatToWxImage(Mat& cvImg, wxImage& wxImg) const
{
    try
    {
        // data dimension
        int w = cvImg.cols;
        int h = cvImg.rows;
        int size = w * h * 3 * sizeof(unsigned char);

        // allocate memory for internal wxImage data
        unsigned char* wxData = (unsigned char*)malloc(size);

        // the matrix stores BGR image for conversion
        Mat cvRGBImg = Mat(h, w, CV_8UC3, wxData);
        switch (cvImg.channels())
        {
        case 3: // 3-channel case: swap R&B channels
        {
            int mapping[] = { 0,2,1,1,2,0 }; // CV(BGR) to WX(RGB)
            mixChannels(&cvImg, 1, &cvRGBImg, 1, mapping, 3);
        } break;

        default:
        {
        }
        }

        wxImg.Destroy(); // free existing data if there's any
        wxImg = wxImage(w, h, wxData);
    }
    catch (...)
    {
        return false;
    }

    return true;
}