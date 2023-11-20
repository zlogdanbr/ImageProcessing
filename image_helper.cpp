#include "image_helper.h"

std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}

void CImageHelper::clean()
{
    original_initiated = false;

    if (Final_ImageOpenCVFormat.empty() == false)
    {
        Final_ImageOpenCVFormat.deallocate();
    }
    if (Original_ImageOpenCVFormat.empty() == false)
    {
        Original_ImageOpenCVFormat.deallocate();
    }
}

void CImageHelper::SetOriginalNew()
{
    Mat savefinal = Final_ImageOpenCVFormat.clone();
    Mat saveOriginal = Original_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    setOrginalImageOpenCV(savefinal);
    original_initiated = true;

    showImage(saveOriginal, "Original");
    showImage(savefinal, "Final");

}

bool CImageHelper::SaveImage(std::string& Path)
{
    wxString s = convertWxStringToString(Path);
    Mat final_image = getFinalImageOpenCV();
    if (getFinalGray() == false)
    {
        wxImage ImgToSave;
        if (convertOpenCVMatToWxImage(final_image, ImgToSave))
        {
            return ImgToSave.SaveFile(s);
        }
        return false;
    }
    else
    {
        std::string spath = convertWxStringToString(Path);        
        if (saveImage(spath, final_image))
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
        std::unique_ptr<unsigned char> wxData{ new unsigned char(size) };

        if (wxData == nullptr)
        {
            return false;
        }

        // the matrix stores BGR image for conversion
        Mat cvRGBImg = Mat(h, w, CV_8UC3, wxData.get());
        switch (cvImg.channels())
        {
            case 3: // 3-channel case: swap R&B channels
                {
                    int mapping[] = { 0,2,1,1,2,0 }; // CV(BGR) to WX(RGB)
                    mixChannels(&cvImg, 1, &cvRGBImg, 1, mapping, 3);
                } 
                break;
            default:
                break;
        }

        wxImg.Destroy(); // free existing data if there's any
        wxImg = wxImage(w, h, wxData.release());
    }
    catch (...)
    {
        return false;
    }

    return true;
}