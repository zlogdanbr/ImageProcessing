#include "childframe.h"

CMyChildFrame::CMyChildFrame(wxFrame* parent) :wxFrame{ parent, -1, "Custom Mask Input", wxPoint(-1, -1) }
{
    button1->Bind(wxEVT_BUTTON,[&](wxCommandEvent& event)
    {
        // set values
        ;
    });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        // cancel
        Close();
    });

    button3->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        // clear
        grid->ClearGrid();
    });

    grid->CreateGrid(13, 13);
}