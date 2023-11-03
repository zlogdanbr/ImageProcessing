// Relearning WxWidgets using these references:
// 
// https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/
// https://zetcode.com/gui/wxwidgets/
// ---------------------------------------------------------------------------------------------
#include <wx/wx.h>
#include <memory>
 
 
class MyFrame : public wxFrame
{
public:
    MyFrame();
 
private:
    void OnApply(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnExit2(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNothing(wxCommandEvent& event);

    wxToolBar* toolbar1 = nullptr;
    wxTextCtrl* textctrl = nullptr;
};
 
enum
{
    ID_MYAPP = 1,
    wxID_ANY2,  // IDs for the buttons in the toolbar
    wxID_ANY3   // IDs for the buttons in the toolbar
};
 
 
MyFrame::MyFrame():wxFrame(nullptr, wxID_ANY, "My App")
{
    // ------------------------------------------------------
    //          how to create a menu  
    // ------------------------------------------------------
    //std::unique_ptr<wxMenu> menuFile{ new wxMenu };
    //menuFile->Append(   ID_MYAPP,
    //                    "&Apply...\tCtrl-H", 
    //                    "Apply");
    //menuFile->AppendSeparator();
    //menuFile->Append(wxID_EXIT); 
    //std::unique_ptr < wxMenu > menuHelp{ new wxMenu };
    //menuHelp->Append(wxID_ABOUT); 
    //std::unique_ptr <wxMenuBar> menuBar{ new wxMenuBar };
    //menuBar->Append(menuFile.release(), "&File");
    //menuBar->Append(menuHelp.release(), "&Help"); 
    //SetMenuBar( menuBar.release() );
 
    // ------------------------------------------------------
    //          how to create a status bar
    // ------------------------------------------------------
    CreateStatusBar();
    SetStatusText("My WxSkequletonApp");
    

    // ------------------------------------------------------
    //          how to add a toolbar with buttons
    // ------------------------------------------------------
    //wxImage::AddHandler(new wxPNGHandler);
    //wxBitmap exit(wxT("exit.png"), wxBITMAP_TYPE_PNG);
    //wxBitmap closeb(wxT("close.png"), wxBITMAP_TYPE_PNG);
    //toolbar1 = new wxToolBar(this, wxID_ANY);
    //wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    //vbox->Add(toolbar1, 0, wxEXPAND);
    //toolbar1->AddTool(wxID_ANY2, wxT("Exit"), exit);
    //toolbar1->AddTool(wxID_ANY3, wxT("Close"), closeb);
    //toolbar1->Realize();
    //SetSizer(vbox); 
    

    // ---------------------------------------------------------
    //          how to connect Event handlers
    // ---------------------------------------------------------
    //Bind(wxEVT_MENU, &MyFrame::OnApply, this, ID_MYAPP);
    //Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    //Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    //Bind(wxEVT_COMMAND_TOOL_CLICKED, &MyFrame::OnExit2, this, wxID_ANY2);
    //Bind(wxEVT_COMMAND_TOOL_CLICKED, &MyFrame::OnNothing, this, wxID_ANY3);


    // ------------------------------------------------------
    //          how to add text control
    // ------------------------------------------------------
    wxPanel* panel = new wxPanel(this, -1);
    textctrl = new wxTextCtrl(  panel, 
                                -1, 
                                wxT(""), 
                                wxDefaultPosition,//wxPoint(-1, -1),
                                wxSize(100,200),
                                wxTE_MULTILINE);

}

void MyFrame::OnExit2(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnNothing(wxCommandEvent& event)
{
    wxMessageBox("Hope this works",
        "About Skeleton App",
        wxOK | wxICON_INFORMATION);

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
