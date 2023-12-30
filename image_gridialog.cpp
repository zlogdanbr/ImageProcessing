
#include "childframes.h"
#include "savekernel.h"


CGridDialogInput::CGridDialogInput(   wxWindow* parent, 
                        Mat& img,
                        wxWindowID id, 
                        const wxString& title, 
                        const wxPoint& pos, 
                        const wxSize& size, long style) : 
                        wxDialog(parent, id, title, pos, size, style)
{
    final = img.clone();
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer5;
    bSizer5 = new wxBoxSizer(wxVERTICAL);

    button1 = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(button1, 0, wxALL, 5);

    button2 = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(button2, 0, wxALL, 5);

    button3 = new wxButton(this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(button3, 0, wxALL, 5);

    button4 = new wxButton(this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(button4, 0, wxALL, 5);

    button5 = new wxButton(this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer5->Add(button5, 0, wxALL, 5);


    bSizer4->Add(bSizer5, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer6;
    bSizer6 = new wxBoxSizer(wxVERTICAL);

    grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

    // Grid
    grid->CreateGrid(13, 13);
    grid->EnableEditing(true);
    grid->EnableGridLines(true);
    grid->EnableDragGridSize(false);
    grid->SetMargins(0, 0);

    // Columns
    grid->EnableDragColMove(false);
    grid->EnableDragColSize(true);
    grid->SetColLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    // Rows
    grid->EnableDragRowSize(true);
    grid->SetRowLabelAlignment(wxALIGN_CENTER, wxALIGN_CENTER);

    // Label Appearance

    // Cell Defaults
    grid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
    bSizer6->Add(grid, 0, wxALL, 5);


    bSizer4->Add(bSizer6, 1, wxEXPAND, 5);

    this->SetSizer(bSizer4);
    this->Layout();

    this->Centre(wxBOTH);

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // set values
            getGridData();
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

    button4->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            wxFileDialog saveFileDialog(this,
            wxEmptyString,
            wxEmptyString,
            "kernel.dvg",
            "Text Files (*.dvg)|*.dvg|All Files (*.*)|*.*",
            wxFD_SAVE);

            if (saveFileDialog.ShowModal() == wxID_OK)
            {
                wxString spath = saveFileDialog.GetPath();
                std::string path = convertWxStringToString(spath);
                SaveDataToFile(path, grid);
            }

        });

    button5->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            wxFileDialog openFileDialog(this,
            wxEmptyString,
            wxEmptyString,
            wxEmptyString,
            "dvg files(*.dvg) | *.dvg",
            wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (openFileDialog.ShowModal() == wxID_OK)
            {
                wxString path = openFileDialog.GetPath();
                std::string spath = convertWxStringToString(path);
                std::vector<std::vector<double>> obs;
                readCSV(obs, spath);
                LoadDataFromFile(obs, grid);
            }

        });
}

CGridDialogInput::~CGridDialogInput()
{
}

void CGridDialogInput::getGridData()
{

    std::vector<std::vector<double>> data;

    wxGridTableBase* wxData = grid->GetTable();

    for (int i = 0; i < wxData->GetRowsCount(); i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < wxData->GetColsCount(); j++)
        {
            try
            {
                auto v = wxData->GetValue(i, j);
                if (wxData->IsEmptyCell(i, j) == false)
                {
                    std::stringstream ss;
                    ss << v;
                    tmp.push_back(std::stod(ss.str()));
                }
            }
            catch (...)
            {
                return;
            }

        }
        if (tmp.size() != 0)
        {
            data.push_back(tmp);
        }
    }

    int kernel_size1 = data.size();
    int kernel_size2 = data[0].size();
    cv::Mat kernel(kernel_size1, kernel_size2, CV_32F, cv::Scalar(0.0));

    if (kernel_size1 == kernel_size2)
    {
        if (kernel_size1 % 2)
        {
            if (kernel_size1 > 2)
            {
                for (int i = 0; i < kernel_size1; i++)
                {
                    for (int j = 0; j < kernel_size1; j++)
                    {
                        kernel.at<float>(i, j) = data[i][j];
                    }
                }
                final = ApplyCustomKernel(final, kernel);
            }
        }
    }
}

