#pragma once

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


    bool setOriginalImage(wxString& path)
    {
        original = getImagePath(path);
        return original.Exists();
    }

    bool setFinalImage(wxString& path)
    {
        finalimage = getImagePath(path);
        return finalimage.Exists();
    }

    wxFileName  original;
    wxFileName  finalimage;

    Mat Final_ImageOpenCVFormat;

    // ------------------------------------------------------------------------------------------------------------
    // https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
    // ------------------------------------------------------------------------------------------------------------

    bool convertOpenCVMatToWxImage(Mat& cvImg, wxImage& wxImg) const
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

    wxImage convertOpenCVMatGrayToWxigets(Mat& pictureMatrix) const
    {
        cv::Mat grayOutput, rgbOutput;
        pictureMatrix.convertTo(grayOutput, CV_8U);
        cvtColor(grayOutput, rgbOutput, 8); // note the BGR here. 
        //If on Linux, set as RGB
        wxImage test(rgbOutput.cols, rgbOutput.rows, rgbOutput.data, true);
        return test;
    }

    bool SaveImage(wxString& Path, bool ifGray = false)
    {
        wxImage ImgToSave;
        if (ifGray == false)
        {
            if (convertOpenCVMatToWxImage(Final_ImageOpenCVFormat, ImgToSave))
            {
                return ImgToSave.SaveFile(Path);
            }
        }
        else
        {
            ImgToSave = convertOpenCVMatGrayToWxigets(Final_ImageOpenCVFormat);
            if (ImgToSave.IsOk())
            {
                return ImgToSave.SaveFile(Path);
            }
        }
        return false;
    }


private:

    wxFileName getImagePath(wxString& path)
    {
        wxFileName imagePath{ wxStandardPaths::Get().GetExecutablePath() };
        imagePath.SetFullName(path);
        return imagePath;
    }


};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:

    wxMenuBar* mainMenu = new wxMenuBar();

    inline static const int ALGO1 = 1;
    inline static const int ALGO2 = 2;

    //--------------------------------------------------------------
    // components---------------------------------------------------
    //--------------------------------------------------------------

    wxPanel* main_panel = nullptr;
    wxStaticBitmap* staticBitmap1 = nullptr;
    CImageHelper images_map{};

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnAlgo1(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);

};



class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

