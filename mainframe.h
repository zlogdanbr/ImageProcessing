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


std::string convertWxStringToString(const wxString wsx);

class CImageMaps final
{
public:
    CImageMaps() = default;


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
    CImageMaps images_map{};

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnAlgo1(wxCommandEvent& event);

};



class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

