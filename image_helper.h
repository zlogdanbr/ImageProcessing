//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#ifndef _IMAGE_HELPER_FUNC_
#define _IMAGE_HELPER_FUNC_

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/artprov.h>
#include <opencv2/highgui/highgui.hpp>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include "filesys.h"
#include "opcvwrapper.h"
#include <functional>


class ImageCache
{
public:

    ImageCache() = default;

    void AddImgToCache(Mat& img)
    {
        image_cache.push(img);
    }

    bool getFirstOfCache(Mat& img)
    {
        Mat top;
        image_cache.pop();
        if (image_cache.empty())
        {
            return false;
        }
        img = image_cache.top();
        return true;
    }

    bool isEmpty()
    {
        return image_cache.empty();
    }
private:

    std::stack<Mat> image_cache;

};


std::string convertWxStringToString(const wxString wsx);

class CImageHelper final
{
public:

    CImageHelper() = default;

    ~CImageHelper()
    {
        clean();
    }

    bool setOriginalImage(std::string& path)
    {
        original = path;
        original_initiated = file_exists(path);
        return original_initiated;
    }


    std::string getOriginalImage() { return original; };

    Mat getOrginalImageOpenCV() const { return Original_ImageOpenCVFormat; };
    Mat getFinalImageOpenCV() const { return Final_ImageOpenCVFormat; };
    void setOrginalImageOpenCV(Mat& m) { Original_ImageOpenCVFormat = m; };
    void setFinalImageOpenCV(Mat& m) { Final_ImageOpenCVFormat = m; };
    bool getOriginalImageInitiated() const { return original_initiated; };

    void clean();
    bool SaveImage(std::string& Path);
    void setFinalGray(bool b) { final_isgray = b; };
    const bool getFinalGray() const { return final_isgray; };
    Mat SetOriginalNew();
    void setOriginalInfact(Mat& m) { _Save_Original_ = m; };
    Mat  getOriginalInFact() { return _Save_Original_; };

private:
    CImageHelper(CImageHelper&) = delete;
    CImageHelper& operator=(CImageHelper&) = delete;
    Mat Final_ImageOpenCVFormat;
    Mat Original_ImageOpenCVFormat;
    bool original_initiated = false;
    bool final_isgray = false;
    std::string original = "";

    Mat _Save_Original_;
};
#endif
//--------------------------------------------------------------------------------------------------