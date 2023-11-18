#ifndef _MAIN_FRAME_MY_APP_DVG_
#define _MAIN_FRAME_MY_APP_DVG_

//--------------------------------------------------------------------------------------------------
// Main frame of the application, a rather simple WxWigets UI
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// References:
// 
// https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/
// https://zetcode.com/gui/wxwidgets/
// https://wiki.wxwidgets.org/Avoiding_Memory_Leaks
// https://github.com/gammasoft71/Examples_wxWidgets
// https://github.com/gammasoft71/Examples_wxWidgets/blob/master/src/MenusAndToolbars/MainMenu/MainMenu.cpp
// https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
// https://stackoverflow.com/questions/8177747/converting-images-between-opencv-and-wxwidgets
// https://docs.opencv.org/4.x/d0/dd4/tutorial_dnn_face.html
// https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
// ---------------------------------------------------------------------------------------------

#include "image_helper.h"
#include "childframe.h"
#include "logs.h"
#include <chrono>


class MyFrame : public wxFrame
{
public:

    MyFrame();

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

    template<typename F>
    void
        ApplyAlgorithm(F& f, bool Gray);

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnOpen(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnNoduleRec(wxCommandEvent& event);
    void onFlipV(wxCommandEvent& event);
    void onFlipH(wxCommandEvent& event);
    void onFlipA(wxCommandEvent& event);
    void onThreshold(wxCommandEvent& event);

    void onCustomKernel(wxCommandEvent& event);
    void onFaces(wxCommandEvent& event);

    void onHelpFile(wxCommandEvent& event);

    void onAllMenu(wxCommandEvent& event);

    void onKeepFinalActive(wxCommandEvent& event);
    void onNoKeepFinalActive(wxCommandEvent& event);

    enum  Opt 
    {
            ALGO_NODE_REC = 1,
            FLIP_H ,
            FLIP_V ,
            FLIP_B ,
            CUSTKERNEL,
            FACE_DETEC,
            ONE_ID_TO_ALL,
            THRESHOLD_FINAL,
            FINAL_KEEP,
            FINAl_KEEP_NO
    };

    void BinAllEvents()
    {
        Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
        Bind(wxEVT_MENU, &MyFrame::OnNoduleRec, this, ALGO_NODE_REC);
        Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE);
        Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
        Bind(wxEVT_MENU, &MyFrame::onFlipV, this, FLIP_H);
        Bind(wxEVT_MENU, &MyFrame::onFlipH, this, FLIP_V);
        Bind(wxEVT_MENU, &MyFrame::onFlipA, this, FLIP_B);       
        Bind(wxEVT_MENU, &MyFrame::onKeepFinalActive, this, FINAL_KEEP);
        Bind(wxEVT_MENU, &MyFrame::onNoKeepFinalActive, this, FINAl_KEEP_NO);
        Bind(wxEVT_MENU, &MyFrame::onThreshold, this, THRESHOLD_FINAL);
        Bind(wxEVT_MENU, &MyFrame::onCustomKernel, this, CUSTKERNEL);
        Bind(wxEVT_MENU, &MyFrame::onHelpFile, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MyFrame::onAllMenu, this, ONE_ID_TO_ALL);        
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL, "Base Algorithms", "Base Algorithms");
        auto menuThresholdL = menuAlgo->Append(THRESHOLD_FINAL, "Apply Threshold", "Apply Threshold");
        menuAlgo->AppendSeparator();
        auto menuFlipH = menuAlgo->Append(FLIP_H, "Flip Image Horizontal", "lip Image Horizontal");
        auto menuFlipV = menuAlgo->Append(FLIP_V, "Flip Image Vertical", "Flip Image Vertical");
        auto menuFlip = menuAlgo->Append(FLIP_B, "Flip Image", "Flip Image");
        auto menuKeep = menuAlgo->Append(FINAL_KEEP, "Keep Final Active", "Keep Final Active");
        auto menuNoKeep = menuAlgo->Append(FINAl_KEEP_NO, "Do not Keep Final Active", "Do not Keep Final Active");

    }
};
#endif
