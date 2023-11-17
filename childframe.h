#pragma once
#include "image_helper.h"
#include <wx/grid.h>
#include "opcvwrapper.h"
#include "logs.h"

class CGridInputDialog : public wxFrame
{
public:

    CGridInputDialog(wxFrame* parent);
    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l ) { outxt = l; };
    void getGridData() const;

private:

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };

    //--------------------------------------------------------------
    // Components
    //--------------------------------------------------------------
    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1{ new wxPanel(basePanel) };
    wxPanel* panel2{ new wxPanel(basePanel, -1) };

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxVERTICAL) };
    wxBoxSizer* hbox1{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* hbox2{ new wxBoxSizer(wxHORIZONTAL) };

    wxButton* button1{ new wxButton(panel1, wxID_ANY, "OK")};
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel")};
    wxButton* button3{ new wxButton(panel1, wxID_ANY, "Clear")};

    // https://docs.wxwidgets.org/3.0/overview_grid.html
    wxGrid* grid = { new wxGrid(
                                    panel2,
                                    -1,
                                    wxPoint(-1,-1),
                                    wxSize(-1,-1),
                                    wxEXPAND
                                )
                    };
    
    // https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
    void setControlslayout()
    {
        // set base sizer
        basePanel->SetSizer(baseSizer);

        // add buttons to the horizontal box
        hbox1->Add(button1);
        hbox1->Add(button2);
        hbox1->Add(button3);

        // add buttons to the horizontal box
        hbox2->Add(grid);

        // set horizontal base sizer at panel1 and panel2
        panel1->SetSizer(hbox1);
        panel2->SetSizer(hbox2);

        // add panel1 to the base sizer at the base panel
        baseSizer->Add(panel1);
        baseSizer->Add(panel2);

        grid->CreateGrid(13, 13);

        Center();
    }

};
