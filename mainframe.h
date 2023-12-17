//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#ifndef _MAIN_FRAME_MY_APP_DVG_
#define _MAIN_FRAME_MY_APP_DVG_

#include "image_helper.h"
#include "childframes.h"
#include "logs.h"
#include <chrono>
#include "settings_dialog.cpp.h"


class MyFrame : public wxFrame
{
public:

    MyFrame();
    ~MyFrame()
    {

    }

    Settings _settings;

private:

    wxMenuBar* mainMenu = new wxMenuBar();

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

    CWriteLogs outxt{ textCtrl };    

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void onCustomKernel(wxCommandEvent& event);
    void onAllMenu(wxCommandEvent& event);
    void onHelpFile(wxCommandEvent& event);
    void onRevert(wxCommandEvent& event);
    void onSelectRoi(wxCommandEvent& event);
    void onCompare(wxCommandEvent& event);

    void OnAdjustContrast(wxCommandEvent& event);
    void OnAdjusyBright(wxCommandEvent& event);
    void OnAdjustGama(wxCommandEvent& event);
    void OnAdjustThreshold(wxCommandEvent& event);

    void OnSettings(wxCommandEvent& event);

    enum  Opt 
    {
        CUSTKERNEL,
        ONE_ID_TO_ALL,
        IMAGE_DISPLAY,
        SUM_IMG_ID,
        SUB_IMG_ID,
        CONTRAST_ID_IMG,
        REVETR_IMG_ID,
        SELECT_ROI,
        COMPARE_IMG,
        ADJUST_CONTRAST,
        ADJUST_BRIGHT,
        ADJUST_GAMA,
        ADJUST_THRESHOLD,
        SETTINGS_ID_DIALOG
    };

    void BinAllEvents()
    {
        Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
        Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE);
        Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);     
        Bind(wxEVT_MENU, &MyFrame::onCustomKernel, this, CUSTKERNEL);
        Bind(wxEVT_MENU, &MyFrame::onHelpFile, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MyFrame::onAllMenu, this, ONE_ID_TO_ALL);  
        Bind(wxEVT_MENU, &MyFrame::onRevert, this, REVETR_IMG_ID);
        Bind(wxEVT_MENU, &MyFrame::onSelectRoi, this, SELECT_ROI);
        Bind(wxEVT_MENU, &MyFrame::onCompare, this, COMPARE_IMG);

        Bind(wxEVT_MENU, &MyFrame::OnAdjustContrast, this, ADJUST_CONTRAST);
        Bind(wxEVT_MENU, &MyFrame::OnAdjusyBright, this, ADJUST_BRIGHT);
        Bind(wxEVT_MENU, &MyFrame::OnAdjustGama, this, ADJUST_GAMA);
        Bind(wxEVT_MENU, &MyFrame::OnAdjustThreshold, this, ADJUST_THRESHOLD);

        Bind(wxEVT_MENU, &MyFrame::OnSettings, this, SETTINGS_ID_DIALOG);
        
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL,  "Algorithms", "Base Algorithms");
        menumenuALL->SetBitmap(wxArtProvider::GetBitmap( wxART_HELP_BOOK, wxART_MENU));
        
        auto menuCtsKernl = menuAlgo->Append(CUSTKERNEL,    "Kernels", "Input custom kernel");
        menuCtsKernl->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
        
        auto menuRevert = menuAlgo->Append(REVETR_IMG_ID, "Revert", "Revert");
        menuRevert->SetBitmap(wxArtProvider::GetBitmap(wxART_UNDO, wxART_MENU));

        auto menuContrast = menuAlgo->Append(ADJUST_CONTRAST, "Adjust contrast", "Adjust contrast");
        auto menuBright = menuAlgo->Append(ADJUST_BRIGHT, "Adjust Brightness", "Adjust Brightness");
        auto menuGama = menuAlgo->Append(ADJUST_GAMA, "Adjust Gamma", "Adjust Gamma");
        auto menuThreshold = menuAlgo->Append(ADJUST_THRESHOLD, "Threshold", "Threshold");
        auto menuSettings = menuAlgo->Append(SETTINGS_ID_DIALOG, "Algorithm Settings", "Algorithm Settings");

        auto menuSelectRoi = menuAlgo->Append(SELECT_ROI, "Select Roi", "Select Roi"); 
        menuSelectRoi->SetBitmap(wxArtProvider::GetBitmap(wxART_COPY, wxART_MENU));

        auto menuCompare = menuAlgo->Append(COMPARE_IMG, "Compare", "Compare");
    }

    void saveShowImage(Mat& img);

};


#endif
