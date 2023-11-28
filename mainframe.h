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
    void onDisplayImage(wxCommandEvent& event);
    void onRevert(wxCommandEvent& event);

    void onSumImage(wxCommandEvent& event);
    void onSubImage(wxCommandEvent& event);

    void onAdjustContrast(wxCommandEvent& event);

    enum  Opt 
    {
        CUSTKERNEL,
        ONE_ID_TO_ALL,
        IMAGE_DISPLAY,
        SUM_IMG_ID,
        SUB_IMG_ID,
        CONTRAST_ID_IMG,
        REVETR_IMG_ID
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
        Bind(wxEVT_MENU, &MyFrame::onDisplayImage, this, IMAGE_DISPLAY);
        Bind(wxEVT_MENU, &MyFrame::onSumImage, this, SUM_IMG_ID);
        Bind(wxEVT_MENU, &MyFrame::onSubImage, this, SUB_IMG_ID);
        Bind(wxEVT_MENU, &MyFrame::onAdjustContrast, this, CONTRAST_ID_IMG);
        Bind(wxEVT_MENU, &MyFrame::onRevert, this, REVETR_IMG_ID);
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo)
    {
        auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL,  "Algorithms", "Base Algorithms");
        menumenuALL->SetBitmap(wxArtProvider::GetBitmap( wxART_HELP_BOOK, wxART_MENU));
        
        auto menuCtsKernl = menuAlgo->Append(CUSTKERNEL,    "Kernels", "Input custom kernel");
        menuCtsKernl->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));
        
        auto menuImage = menuAlgo->Append(IMAGE_DISPLAY,    "Edit", "Image Display");
        menuImage->SetBitmap(wxArtProvider::GetBitmap (wxART_INFORMATION, wxART_MENU));

        // TODO
        //auto menuSum = menuAlgo->Append(SUM_IMG_ID, "Sum images", "Sum images");
        //menuSum->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MENU));

        //auto menuSub = menuAlgo->Append(SUB_IMG_ID, "Subtrack images", "Subtrack images");
        //menuSub->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MENU));

        //auto menuContrast = menuAlgo->Append(CONTRAST_ID_IMG, "Adjust Contrast", "Adjust Contrast");
        //menuContrast->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT, wxART_MENU));

        auto menuRevert = menuAlgo->Append(REVETR_IMG_ID, "Revert", "Revert");
        menuRevert->SetBitmap(wxArtProvider::GetBitmap(wxART_UNDO, wxART_MENU));
    }

    int getContrastDialog()
    {
        wxNumberEntryDialog dialog(this, "Contrast Adjustment", "Choose Contrast Adjustment", "Contrast Adjustmen Factor", 1, 1, 100);

        if (dialog.ShowModal() == wxID_OK)
        {
            auto scale = static_cast<int>(dialog.GetValue());
            return scale;
        }

        return -1;
    }

};
#endif
