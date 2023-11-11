#pragma once
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

    wxFileName getImagePath(wxString& path)
    {
        wxFileName imagePath{ wxStandardPaths::Get().GetExecutablePath() };
        imagePath.SetFullName(path);
        return imagePath;
    }

    bool setOriginalImage(wxString& path)
    {
        original = getImagePath(path);
        original_initiated = original.Exists();
        return original_initiated;
    }

    bool setFinalImage(wxString& path)
    {
        finalimage = getImagePath(path);
        final_initiated = finalimage.Exists();
        return final_initiated;
    }

    wxFileName getOriginalImage()const { return original; };
    wxFileName getFinalImage()const { return finalimage; };

    Mat getOrginalImageOpenCV() const { return Original_ImageOpenCVFormat; };
    Mat getFinalImageOpenCV() const { return Final_ImageOpenCVFormat; };

    void setOrginalImageOpenCV(Mat& m) { Original_ImageOpenCVFormat = m; };
    void setFinalImageOpenCV(Mat& m) { Final_ImageOpenCVFormat = m; };

    bool getOriginalImageInitiated() const { return original_initiated; };
    bool getFinallImageInitiated() const { return final_initiated; }


    void clean()
    {
        original.Clear();
        finalimage.Clear();
        original_initiated = false;
        final_initiated = false;
        final_isgray = false;
        if (Final_ImageOpenCVFormat.empty() == false)
        {
            Final_ImageOpenCVFormat.deallocate();
        }
        if (Original_ImageOpenCVFormat.empty() == false)
        {
            Original_ImageOpenCVFormat.deallocate();
        }
    }

    bool SaveImage(wxString& Path);

    void setFinalGray(bool b) { final_isgray = b; };
    const bool getFinalGray() const { return final_isgray; };

private:

    CImageHelper(CImageHelper&) = delete;
    CImageHelper& operator=(CImageHelper&) = delete;

    wxFileName  original;
    wxFileName  finalimage;

    Mat Final_ImageOpenCVFormat;
    Mat Original_ImageOpenCVFormat;

    bool original_initiated = false;
    bool final_initiated = false;

    bool final_isgray = false;

    // ------------------------------------------------------------------------------------------------------------
    // https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
    // ------------------------------------------------------------------------------------------------------------
    bool convertOpenCVMatToWxImage(Mat& cvImg, wxImage& wxImg) const;


};
//--------------------------------------------------------------------------------------------------