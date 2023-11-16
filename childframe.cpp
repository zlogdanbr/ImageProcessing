#include "childframe.h"

CGridInputDialog::CGridInputDialog(wxFrame* parent):wxFrame{ parent, -1, "Custom Mask Input", wxPoint(-1, -1), wxSize(1160, 450) }
{
    button1->Bind(wxEVT_BUTTON,[&](wxCommandEvent& event)
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

    grid->CreateGrid(13, 13);
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
                Mat out = ApplyCustom2Dfilter(img_orig, kernel);

                if (out.empty() == false)
                {
                    imghelper->setFinalImageOpenCV(out);
                    showImage(out, "Final");
                    imghelper->setFinalGray(true);
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