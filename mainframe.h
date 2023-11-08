#pragma once

#include <wx/wx.h>
#include <memory>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

using MenuContainer = std::map<wxString, wxMenu*>;


class MyFrame : public wxFrame
{
public:
    MyFrame();

private:

    //--------------------------------------------------------------
    // components---------------------------------------------------
    //--------------------------------------------------------------

    wxMenu* fmenu = nullptr;
    wxMenu* fhelp = nullptr;
    wxMenuBar* menubar = nullptr;
    wxPanel* main_panel = nullptr;

    //--------------------------------------------------------------
    // Data containers
    //---------------------------------------------------------------

    MenuContainer menus;// container of menus

    //---------------------------------------------------------------
    // helper functions----------------------------------------------
    //---------------------------------------------------------------
    
    wxMenu* setMenu(int ID, wxString&& item, wxString&& helpertext );// set a menu     
    wxMenuBar* setAppMenuBar();// set a menu bar

    std::string convertWxStringToString(const wxString wsx) const
    {
        std::stringstream s;
        s << wsx;
        return s.str();
    }

    //---------------------------------------------------------------
    // event handlers------------------------------------------------
    //---------------------------------------------------------------
    void OnApply(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_MYAPP = 1,
    wxID_ANY2,  // IDs for the buttons in the toolbar
    wxID_ANY3,  // IDs for the buttons in the toolbar
    wxID_BUTTONOK,
    wxID_BUTTONCANEL
};


class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

