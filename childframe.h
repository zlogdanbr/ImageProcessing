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
    // components---------------------------------------------------
    //--------------------------------------------------------------
    CImageHelper* imghelper;
    CWriteLogs* outxt;
    wxPanel* panel{ new wxPanel(this) };
    wxButton* button1{ new wxButton(panel, wxID_ANY, "OK",      { 5,   10 }) };
    wxButton* button2{ new wxButton(panel, wxID_ANY, "Cancel",  { 100, 10 }) };
    wxButton* button3{ new wxButton(panel, wxID_ANY, "Clear",   { 195, 10 }) };

    // https://docs.wxwidgets.org/3.0/overview_grid.html
    wxGrid* grid = { new wxGrid(
                                    panel,
                                    -1,
                                    wxPoint(5, 50),
                                    wxSize(1160, 270)

                                )
                    };

};