//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#ifndef _MAIN_FRAME_MY_APP_DVG_
#define _MAIN_FRAME_MY_APP_DVG_

#include <chrono>
#include "image_helper.h"
#include "childframes.h"
#include "logs.h"


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
    void onAllMenu(wxCommandEvent& event);
    void onHelpFile(wxCommandEvent& event);
    void onApplySIFT(wxCommandEvent& event);
    void onImageDraw(wxCommandEvent& event);
    void onApplyTemplate(wxCommandEvent& event);
    void onApplyTemplateFull(wxCommandEvent& event);
    void onApplyHuh(wxCommandEvent& event);

    enum  Opt 
    {
        ONE_ID_TO_ALL,
        IMAGE_DISPLAY,
        SUM_IMG_ID,
        SUB_IMG_ID,
        CONTRAST_ID_IMG,
        MAKE_SAME_SIZE,
        DRAW_ON_IMAGE,
        TEMPLATE_ID,
        TEMPLATE_ID_FULL,
        HUHID,
    };

    void BinAllEvents()
    {
        Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
        Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE);
        Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);     
        Bind(wxEVT_MENU, &MyFrame::onHelpFile, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MyFrame::onAllMenu, this, ONE_ID_TO_ALL);  
        Bind(wxEVT_MENU, &MyFrame::onApplySIFT, this, MAKE_SAME_SIZE);
        Bind(wxEVT_MENU, &MyFrame::onImageDraw, this, DRAW_ON_IMAGE);
        Bind(wxEVT_MENU, &MyFrame::onApplyTemplate, this, TEMPLATE_ID); 
        Bind(wxEVT_MENU, &MyFrame::onApplyTemplateFull, this, TEMPLATE_ID_FULL);
        Bind(wxEVT_MENU, &MyFrame::onApplyHuh, this, HUHID);
                
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL,  "Algorithms", "Base Algorithms");
        menumenuALL->SetBitmap(wxArtProvider::GetBitmap(wxART_HELP_BOOK, wxART_MENU));
        
        auto menuSameSize= menuAlgo->Append(MAKE_SAME_SIZE, "SIFT Algorithm Comparison", "Image Space");
        menuSameSize->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_MENU));

        auto menuTemplate = menuAlgo->Append(TEMPLATE_ID, "Global Template Matching", "Image Space");
        menuTemplate->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_MENU));

        auto menuTemplateFull = menuAlgo->Append(TEMPLATE_ID_FULL, "Local Template Matching", "Image Space");
        menuTemplateFull->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_MENU));

        auto menuHuh = menuAlgo->Append(HUHID, "Compare using Huh Moments", "Image Space");
        menuHuh->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_MENU));

    }
};
#endif
