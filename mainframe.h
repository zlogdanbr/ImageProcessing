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

class MyFrame : public wxFrame
{
public:

    MyFrame();

private:

    wxMenuBar* mainMenu = new wxMenuBar();

    inline static const int ALGO_NODE_REC = 1;
    inline static const int ALGO_GRAY_C = 2;
    inline static const int ALGO_EQUALIZE= 3;
    inline static const int ALGO_LAPLACIAN = 4;

    //--------------------------------------------------------------
    // components---------------------------------------------------
    //--------------------------------------------------------------

    wxPanel* main_panel = nullptr;
    wxStaticBitmap* staticBitmap1 = nullptr;
    CImageHelper ImageHelper{};
    wxTextCtrl* textCtrl{ new wxTextCtrl(
                                            this, 
                                            wxID_ANY, 
                                            "", 
                                            wxDefaultPosition, 
                                            wxDefaultSize, 
                                            wxTE_MULTILINE
                                        ) 
                        };

    template<typename F>
    void
        ApplyAlgorith(F& f, bool Gray);

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnNoduleRec(wxCommandEvent& event);
    void OnDoGrayScale(wxCommandEvent& event);
    void OnDoEqualize(wxCommandEvent& event);
    void OnDoLaplacian(wxCommandEvent& event);

};


