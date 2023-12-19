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


class MyFrame : public wxFrame
{
public:

    MyFrame();
    ~MyFrame()
    {

    }

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
    void onMakeSameSize(wxCommandEvent& event);

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
        MAKE_SAME_SIZE
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
        Bind(wxEVT_MENU, &MyFrame::onMakeSameSize, this, MAKE_SAME_SIZE);

        
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL,  "Algorithms", "Base Algorithms");
        menumenuALL->SetBitmap(wxArtProvider::GetBitmap( wxART_HELP_BOOK, wxART_MENU));
        
        auto menuCtsKernl = menuAlgo->Append(CUSTKERNEL,    "Kernels", "Input custom kernel");
        menuCtsKernl->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
        
        auto menuRevert = menuAlgo->Append(REVETR_IMG_ID, "Revert", "Revert");
        menuRevert->SetBitmap(wxArtProvider::GetBitmap(wxART_UNDO, wxART_MENU));

        auto menuSelectRoi = menuAlgo->Append(SELECT_ROI, "Select Roi", "Select Roi"); 
        menuSelectRoi->SetBitmap(wxArtProvider::GetBitmap(wxART_COPY, wxART_MENU));

        auto menuCompare = menuAlgo->Append(COMPARE_IMG, "Show Difference", "Show Difference");

        auto menuSameSize= menuAlgo->Append(MAKE_SAME_SIZE, "Compare Two Images", "Compare Two Images");
    }



};
#endif
