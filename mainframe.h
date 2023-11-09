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

    wxFileName getImagePath(wxString& path)
    {
        wxFileName imagePath{ wxStandardPaths::Get().GetExecutablePath() };
        imagePath.SetFullName(path);
        return imagePath;
    }

    bool setOriginalImage(wxString& path)
    {
        original = getImagePath(path);
        original_initiated =  original.Exists();
        return original_initiated;
    }

    bool setFinalImage(wxString& path)
    {
        finalimage = getImagePath(path);
        final_initiated = finalimage.Exists();
        return final_initiated;
    }

    wxFileName  original;
    wxFileName  finalimage;

    Mat Final_ImageOpenCVFormat;
    Mat Original_ImageOpenCVFormat;

    bool original_initiated = false;
    bool final_initiated = false;

    void clean()
    {
        original.Clear();
        finalimage.Clear();
        original_initiated = false;
        final_initiated = false;
        if (Final_ImageOpenCVFormat.empty() == false)
        {
            Final_ImageOpenCVFormat.deallocate();
        }
    }

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

    bool SaveImage(wxString& Path, bool ifGray = false)
    {
        wxImage ImgToSave;
        if (convertOpenCVMatToWxImage(Final_ImageOpenCVFormat, ImgToSave))
        {
            return ImgToSave.SaveFile(Path);
        }       
        return false;
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
    wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, "textBox", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnAlgo1(wxCommandEvent& event);
    void OnAlgo2(wxCommandEvent& event);

};


