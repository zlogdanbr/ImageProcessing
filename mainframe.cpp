
#include "mainframe.h"

wxMenu* MyFrame::setMenu(int ID, wxString&& item, wxString&& helpertext)
{
    wxMenu* menu{ new wxMenu };
    menu->Append(   ID,
                        item,//"&Apply...\tCtrl-H",
                        helpertext//"Apply"
                    );
    menu->AppendSeparator();

    return menu;
}

wxMenuBar* MyFrame::setAppMenuBar()
{
    wxMenuBar* menuBar{ new wxMenuBar };

    for ( auto it = menus.begin(); it != menus.end(); it++)
    {
        wxString wxs = it->first;
        wxMenu* wxm  = it->second;

        menuBar->Append( wxm, wxs);
    }

    return menuBar;
}


MyFrame::MyFrame() :wxFrame(NULL, -1, "My SkeletonApp", wxPoint(-1, -1), wxSize(250, 180))
{
    // ------------------------------------------------------
    //          how to create a menu  
    // ------------------------------------------------------
    fmenu = setMenu(ID_MYAPP, "&Apply...\tCtrl-H", "Apply");
    fmenu->Append(wxID_EXIT);
    fhelp = setMenu(wxID_ABOUT, "About", "About");
    
    menus["&File"] = fmenu;
    menus["&Help"] = fhelp;

    menubar = setAppMenuBar();
    SetMenuBar(menubar);

    // ------------------------------------------------------
    //          how to create a status bar
    // ------------------------------------------------------
    CreateStatusBar();
    SetStatusText("My WxSkequletonApp");


    // ---------------------------------------------------------
    //          how to connect Event handlers
    // ---------------------------------------------------------
    Bind(wxEVT_MENU, &MyFrame::OnApply, this, ID_MYAPP);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    


    // ------------------------------------------------------
    //          how to add text control
    // ------------------------------------------------------

    wxColour col1;
    col1.Set(wxT("#ededed"));
    vbox = new wxBoxSizer(wxVERTICAL);
    panel1 = new wxPanel(this, -1);
    panel1->SetBackgroundColour(col1);
    panel1->SetSizer(vbox);
    
    Centre();
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