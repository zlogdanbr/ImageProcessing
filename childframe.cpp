#include "childframe.h"

CMyChildFrame::CMyChildFrame(wxFrame* parent) :wxFrame{ parent, -1, "Custom Mask Input", wxPoint(-1, -1) }
{
    button1->Bind(wxEVT_BUTTON,[&](wxCommandEvent& event)
    {
        // set values
        auto Kernel = getGridData();
        Close();
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


std::vector<std::vector<double>> CMyChildFrame::getGridData() const
{

    wxGridTableBase* wxData = grid->GetTable();
    std::vector<std::vector<double>> data;

    for (int i = 0; i < wxData->GetRowsCount(); i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < wxData->GetColsCount(); j++)
        {
            auto v = wxData->GetValue(i, j);
            if (wxData->IsEmptyCell(i, j) == false)
            {
                std::stringstream ss;
                ss << v;
                tmp.push_back(std::stod(ss.str()));
            }
        }
        if (tmp.size() != 0)
        {
            data.push_back(tmp);
        }
    }

    return data;
    
}