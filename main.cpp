//--------------------------------------------------------------------------------------------------
// Main frame of the application, a rather simple WxWigets UI
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
//      Version:            1.0.1
//      Description:        Image Processing Tool
//      web:                http://zlogdan.wordpress.com
//      Licence:            GNU GENERAL PUBLIC LICENSE GPLv3
//--------------------------------------------------------------------------------------------------
// References:
// 
// https://github.com/gammasoft71/Examples_wxWidgets
// https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/
// https://zetcode.com/gui/wxwidgets/
// https://wiki.wxwidgets.org/Avoiding_Memory_Leaks
// https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
// https://stackoverflow.com/questions/8177747/converting-images-between-opencv-and-wxwidgets
// https://docs.opencv.org/4.x/d0/dd4/tutorial_dnn_face.html
// https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
// ---------------------------------------------------------------------------------------------

#include "mainframe.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

