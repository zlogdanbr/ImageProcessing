#ifndef _IMAGE_HELPER_FUNC_
#define _IMAGE_HELPER_FUNC_
//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/artprov.h>
#include <memory>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include "filesys.h"
#include  "segmentation.h"


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
    void SetOriginalNew();

private:

    CImageHelper(CImageHelper&) = delete;
    CImageHelper& operator=(CImageHelper&) = delete;
    Mat Final_ImageOpenCVFormat;
    Mat Original_ImageOpenCVFormat;
    bool original_initiated = false;
    bool final_isgray = false;
    std::string original = "";


    // ------------------------------------------------------------------------------------------------------------
    // https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
    // ------------------------------------------------------------------------------------------------------------
    bool convertOpenCVMatToWxImage(Mat& cvImg, wxImage& wxImg) const;


};
#endif
//--------------------------------------------------------------------------------------------------