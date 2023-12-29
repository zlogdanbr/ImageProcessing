
#include "childframes.h"
#include "savekernel.h"

void CGridInputDialog::setControlslayout()
{
    // set base sizer
    basePanel->SetSizer(baseSizer);

    // add buttons to the horizontal box
    hbox1->Add(button1);
    hbox1->Add(button2);
    hbox1->Add(button3);
    hbox1->Add(button4);
    hbox1->Add(button5);

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

CGridInputDialog::CGridInputDialog(wxFrame* parent) :CInputDialogBase{ parent,"Custom Kernel Input" }
{
    setControlslayout();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // set values
            getGridData();
            //Close();
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


void CGridInputDialog::getGridData() const
{

    if (imghelper->getOriginalImageInitiated() == false)
    {
        outxt->writeTo("Image not loaded.\n");
        return;
    }
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
                outxt->writeTo("Error in the kernel values.\n");
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

                Mat img_orig = imghelper->getOrginalImageOpenCV();
                Mat out = ApplyCustomKernel(img_orig, kernel);

                if (out.empty() == false)
                {
                    imghelper->setFinalImageOpenCV(out);                    
                    imghelper->setFinalGray(true);
                    imghelper->SetOriginalNew();
                }
                else
                {
                    outxt->writeTo("Error appplying algo.\n");
                }
            }
            else
            {
                outxt->writeTo("Kernels dimension must be higher than 2.\n");
            }
        }
        else
        {
            outxt->writeTo("Kernels must have odd sizes.\n");
        }

    }
    else
    {
        outxt->writeTo("Incorrect kernel size.\n");
    }
}