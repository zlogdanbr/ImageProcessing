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
    void onAllMenu(wxCommandEvent& event);
    void onHelpFile(wxCommandEvent& event);
    void onMakeSameSize(wxCommandEvent& event);
    void onImageDraw(wxCommandEvent& event);

    enum  Opt 
    {
        ONE_ID_TO_ALL,
        IMAGE_DISPLAY,
        SUM_IMG_ID,
        SUB_IMG_ID,
        CONTRAST_ID_IMG,
        MAKE_SAME_SIZE,
        DRAW_ON_IMAGE
    };

    void BinAllEvents()
    {
        Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
        Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE);
        Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);     
        Bind(wxEVT_MENU, &MyFrame::onHelpFile, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MyFrame::onAllMenu, this, ONE_ID_TO_ALL);  
        Bind(wxEVT_MENU, &MyFrame::onMakeSameSize, this, MAKE_SAME_SIZE);
        Bind(wxEVT_MENU, &MyFrame::onImageDraw, this, DRAW_ON_IMAGE);
        
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL,  "Algorithms", "Base Algorithms");
        menumenuALL->SetBitmap(wxArtProvider::GetBitmap(wxART_HELP_BOOK, wxART_MENU));
        
        auto menuSameSize= menuAlgo->Append(MAKE_SAME_SIZE, "SIFT Algorithm Comparison", "Image Space");
        menuSameSize->SetBitmap(wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_MENU));

    }
};
#endif
