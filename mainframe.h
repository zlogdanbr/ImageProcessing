#pragma once

//--------------------------------------------------------------------------------------------------
// Main frame of the application, a rather simple WxWigets UI
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
// If you would like to add a new algorithm to the menu do the following
// 
// 1- Create the ID using:
// inline static const int ALGO_NODE_REC = 1;
// 
// 2- Define the event handler
// void OnNoduleRec(wxCommandEvent& event);
// 
// Inside this method you may either call
//  ApplyAlgorithm(F& f, bool Gray);
//  ApplyAlgorithm(F& f, bool Gray, int kernel_size);
// 
// - set Gray to True if the final image is a gray scale image because the application will use OpenCV
// functions to save. For reasons I am not aware, the OpenCV save functions were not working for colored
// images
// - use the second method if the algorithm you would like to implement uses a kernel
// ( see opcvwrapper.h )
// 
// 3- Add the event to event table
//  Bind(wxEVT_MENU, &MyFrame::OnNoduleRec, this, ALGO_NODE_REC);
// 
// 4- Add the new menu item
//  auto menuItemNode= menuAlgo->Append(ALGO_NODE_REC, "Find Contours\tCTRL+F", "Find Contours")
//--------------------------------------------------------------------------------------------------
// I am working with jpgs images only now but I plan to add options for more formats. Actually you
// can change that option only by changing these lines:
// void MyFrame::OnOpen(wxCommandEvent& event)
//{
//    wxFileDialog openFileDialog(this,
//        wxEmptyString,
//        wxEmptyString,
//        wxEmptyString,
//        "Image files (*.jpg)|*.jpg|All Files (*.*)|*.*",
//        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
//  and 
// 
// void MyFrame::OnSave(wxCommandEvent& event)
//{
//    if (ImageHelper.getOriginalImageInitiated() == true)
//    {
//        auto name_final = ImageHelper.getOriginalImage().GetName();
//        auto path = ImageHelper.getOriginalImage().GetPath();
//        auto tosave = path + "\\" + name_final + "_proc_" + ".jpg";
// 
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
// ---------------------------------------------------------------------------------------------

#include "image_helper.h"
#include "childframe.h"
#include <chrono>

class CWriteLogs
{
public:
    CWriteLogs(wxTextCtrl* ptrtextCtrl):textCtrl{ ptrtextCtrl } {};
    virtual ~CWriteLogs() {};

    void writeTo(wxString& msg)
    {
        textCtrl->AppendText(return_current_time_and_date() + ": " + msg);
    }

    void writeTo(const char* msg)
    {
        textCtrl->AppendText(return_current_time_and_date() + ": " + msg);
    }

    std::string return_current_time_and_date()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }


private:
    CWriteLogs(CWriteLogs&) = delete;
    CWriteLogs& operator=(CWriteLogs&) = delete;
    wxTextCtrl* textCtrl;


};


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

    template<typename F>
    void
        ApplyAlgorithm(F& f, bool Gray, int kernel_size);

    template<typename F>
    void
        ApplyBaseOperationsOnExistent(F& f, bool Gray = true, int kernel_size = 3);


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
    void onImageBlurKernel33(wxCommandEvent& event);
    void onImageBlurKernel55(wxCommandEvent& event);
    void onGaussian(wxCommandEvent& event);
    void onMedian(wxCommandEvent& event);

    void onFlipV(wxCommandEvent& event);
    void onFlipH(wxCommandEvent& event);
    void onFlipA(wxCommandEvent& event);

    void onSumImages(wxCommandEvent& event);
    void onSubImages(wxCommandEvent& event);
    void onXorImages(wxCommandEvent& event);

    void onCustomKernel(wxCommandEvent& event);

    enum  Opt {
            ALGO_NODE_REC = 1,
            ALGO_GRAY_C,
            ALGO_EQUALIZE,
            ALGO_LAPLACIAN,
            ALGO_BLUR33 ,
            ALGO_BLUR55 ,
            ALGO_GAUSSIAN ,
            ALGO_MEDIAN ,
            FLIP_H ,
            FLIP_V ,
            FLIP_B ,
            SUMIMG ,
            SUBIMG ,
            XORIMG,
            CUSTKERNEL,

    };

    void BinAllEvents()
    {
        Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
        Bind(wxEVT_MENU, &MyFrame::OnNoduleRec, this, ALGO_NODE_REC);
        Bind(wxEVT_MENU, &MyFrame::OnDoGrayScale, this, ALGO_GRAY_C);
        Bind(wxEVT_MENU, &MyFrame::OnDoEqualize, this, ALGO_EQUALIZE);
        Bind(wxEVT_MENU, &MyFrame::OnDoLaplacian, this, ALGO_LAPLACIAN);
        Bind(wxEVT_MENU, &MyFrame::onImageBlurKernel33, this, ALGO_BLUR33);
        Bind(wxEVT_MENU, &MyFrame::onImageBlurKernel55, this, ALGO_BLUR55);
        Bind(wxEVT_MENU, &MyFrame::onMedian, this, ALGO_MEDIAN);
        Bind(wxEVT_MENU, &MyFrame::onGaussian, this, ALGO_GAUSSIAN);
        Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE);
        Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
        Bind(wxEVT_MENU, &MyFrame::onFlipV, this, FLIP_H);
        Bind(wxEVT_MENU, &MyFrame::onFlipH, this, FLIP_V);
        Bind(wxEVT_MENU, &MyFrame::onFlipA, this, FLIP_B);
        Bind(wxEVT_MENU, &MyFrame::onSumImages, this, SUMIMG);
        Bind(wxEVT_MENU, &MyFrame::onSubImages, this, SUBIMG);
        Bind(wxEVT_MENU, &MyFrame::onXorImages, this, XORIMG);
        Bind(wxEVT_MENU, &MyFrame::onCustomKernel, this, CUSTKERNEL);
    }

    void AddSubitemsToMenu(wxMenu* menuAlgo);
};


