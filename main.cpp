// https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/
#include <wx/wx.h>
#include <memory>
 
class MyApp : public wxApp
{
public:
    bool OnInit() override;
};
 
wxIMPLEMENT_APP(MyApp);
 
class MyFrame : public wxFrame
{
public:
    MyFrame();
 
private:
    void OnApply(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
 
enum
{
    ID_MYAPP = 1
};
 
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
 
MyFrame::MyFrame():wxFrame(nullptr, wxID_ANY, "My App")
{
    std::unique_ptr<wxMenu> menuFile{ new wxMenu };
    menuFile->Append(   ID_MYAPP,
                        "&Apply...\tCtrl-H", 
                        "Apply");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    std::unique_ptr < wxMenu > menuHelp{ new wxMenu };
    menuHelp->Append(wxID_ABOUT);
 
    std::unique_ptr <wxMenuBar> menuBar{ new wxMenuBar };
    menuBar->Append(menuFile.release(), "&File");
    menuBar->Append(menuHelp.release(), "&Help");
 
    SetMenuBar( menuBar.release() );
 
    CreateStatusBar();
    SetStatusText("My WxSkequletonApp");
 
    Bind(wxEVT_MENU, &MyFrame::OnApply, this, ID_MYAPP);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}
 
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets skeleton app",
                 "About Skeleton App",
                 wxOK | wxICON_INFORMATION);
}
 
void MyFrame::OnApply(wxCommandEvent& event)
{
    wxLogMessage("It should do something!");
}