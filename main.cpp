//--------------------------------------------------------------------------------------------------
// Main application. Do not change it.
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//----------------------------------------------------------------------------------------------

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
