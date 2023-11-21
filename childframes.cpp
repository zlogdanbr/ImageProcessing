#include "childframes.h"
#include "savekernel.h"


COpenImage::COpenImage(CImageHelper* imghelper, CWriteLogs* outxt) :imghelper{ imghelper }, outxt{ outxt }
{

}

void COpenImage::OpenFile(wxFrame& frame)
{
    wxFileDialog openFileDialog(&frame,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "jpg and tif files(*.jpg; *.tif) | *.jpg; *.tif",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    imghelper->clean();
    openFileDialog.SetFilterIndex(0);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = openFileDialog.GetPath();
        std::string spath = convertWxStringToString(path);
        if (imghelper->setOriginalImage(spath))
        {
            Mat img;
            if (loadImage(spath, img) == true)
            {
                imghelper->clean();
                imghelper->setOrginalImageOpenCV(img);
                showImage(imghelper->getOrginalImageOpenCV(), "Original");
                outxt->writeTo("Image loaded correctly\n");
                imghelper->setOriginalImage(spath);
            }
            else
            {
                outxt->writeTo("Error loading Image\n");
            }
        }
        else
        {
            outxt->writeTo("Error loading Image\n");
        }
    }
}

CSaveImage::CSaveImage(CImageHelper* imghelper, CWriteLogs* outxt) :imghelper{ imghelper }, outxt{ outxt }
{

}

void CSaveImage::SaveFile(wxFrame& frame)
{
    if (imghelper->getOriginalImageInitiated() == true)
    {

        wxFileDialog saveFileDialog(&frame,
            wxEmptyString,
            wxEmptyString,
            "MyFile.jpg", "Text Files (*.jpg)|*.jpg|All Files (*.*)|*.*",
            wxFD_SAVE);
        if (saveFileDialog.ShowModal() == wxID_OK)
        {

            wxString spath = saveFileDialog.GetPath();
            std::string path = convertWxStringToString(spath);

            if (imghelper->SaveImage(path))
            {
                outxt->writeTo("Image sucessfully saved as:\n");
                outxt->writeTo(spath + "\n");
            }
            else
            {
                outxt->writeTo("Error saving image.\n");
            }
        }
        else
        {
            outxt->writeTo("Error saving image.\n");
            return;
        }
    }
    else
    {
        outxt->writeTo("Image has not been loaded.\n");
    }
}

CInputDialogBase::CInputDialogBase(wxFrame* parent, wxString name) :wxFrame{ parent, -1, name, wxPoint(-1, -1) }
{
}

void CInputDialog::DoFunction()
{
    wxString opt = getSelectionText();

    Function1Parameter f1 = getAlgoFunctionSimple(opt);

    if (f1 == nullptr)
    {
        Function2Parameter f2 = getAlgoFunctionMore(opt);
        if (f2 == nullptr)
        {
            outxt->writeTo("Error while loading algos.\n");
            return;
        }

        ApplyAlgorithm(f2, true, 5);
    }
    else
    {
        ApplyAlgorithm(f1, true);
    }
}

void
CInputDialogBase::ApplyAlgorithm(Function1Parameter& f, bool Gray)
{

    if (imghelper->getOriginalImageInitiated() == false)
    {
        auto spath = imghelper->getOriginalImage();
        Mat img;
        Mat out;
        imghelper->setFinalGray(Gray);

        if (loadImage(spath, img) == true)
        {
            out = f(img);
            if (out.empty() == false)
            {
                imghelper->setFinalImageOpenCV(out);
                outxt->writeTo("Algorithm applied correctly\n");
                imghelper->SetOriginalNew();
            }
            else
            {
                outxt->writeTo("Algorithm error\n");
            }
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV());
        if (out.empty() == false)
        {
            imghelper->setFinalImageOpenCV(out);
            outxt->writeTo("Algorithm applied correctly\n");
            imghelper->SetOriginalNew();
        }
        else
        {
            outxt->writeTo("Algorithm error\n");
        }
    }


}

void
CInputDialogBase::ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        auto spath = imghelper->getOriginalImage();
        Mat img;
        Mat out;
        imghelper->setFinalGray(Gray);

        if (loadImage(spath, img) == true)
        {
            out = f(img, kernel_size);
            if (out.empty() == false)
            {
                imghelper->setFinalImageOpenCV(out);
                outxt->writeTo("Algorithm applied correctly\n");
                imghelper->SetOriginalNew();
            }
            else
            {
                outxt->writeTo("Algorithm error\n");
            }
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), kernel_size);
        if (out.empty() == false)
        {
            imghelper->setFinalImageOpenCV(out);
            outxt->writeTo("Algorithm applied correctly\n");
            imghelper->SetOriginalNew();
        }
        else
        {
            outxt->writeTo("Algorithm error\n");
        }
    }
}

CImageCustomDialog::CImageCustomDialog(wxFrame* parent) :CInputDialogBase{ parent,"Image Editor" }
{
    setControlslayout();

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        // cancel
        Close();
    });

    button3->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if (image.IsOk())
        {
            wxNumberEntryDialog dialog(this, "Scale Factor", "Choose Scale Factor", "Scale Factor", 2, 2, 10);

            if (dialog.ShowModal() == wxID_OK)
            {
                auto scale = static_cast<int>(dialog.GetValue());
                auto h = image.GetHeight();
                auto w = image.GetWidth();
                auto hn = static_cast<int>(h / scale);
                auto wn = static_cast<int>(w / scale);

                if (hn <= 0)
                {
                    hn = h;
                }

                if (wn <= 0)
                {
                    wn = w;
                }

                wxImage image2 = image.Scale(hn, wn);
                image = image2.Copy();
                image2.Clear();
                reloadImage(hn);
            }
        }

    });

    button6->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if (image.IsOk())
        {
            wxNumberEntryDialog dialog(this, "Scale Factor", "Choose Scale Factor", "Scale Factor", 2, 2, 10);

            if (dialog.ShowModal() == wxID_OK)
            {
                auto scale = static_cast<int>(dialog.GetValue());
                auto h = image.GetHeight();
                auto w = image.GetWidth();
                auto hn = static_cast<int>(h * scale);
                auto wn = static_cast<int>(w * scale);

                if (hn <= 0 || hn > 2000)
                {
                    hn = h;
                }

                if (wn <= 0 || wn > 2000)
                {
                    wn = w;
                }

                wxImage image2 = image.Scale(hn, wn);
                image = image2.Copy();
                image2.Clear();
                reloadImage(hn);
            }
        }
    });

    button4->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if (image.IsOk())
        {
           SaveImage();
        }
            
    });

    button5->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        loadImage();
    });

    button8->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if ( image.IsOk())
        {
            wxImage image2 = image.Rotate90(true);
            image = image2.Copy();
            image2.Clear();
            image2.Destroy();
            reloadImage();
        }
    });

    button9->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if (image.IsOk())
        {
            wxImage image2 = image.Rotate90(false);
            image = image2.Copy();
            image2.Clear();
            image2.Destroy();
            reloadImage();
        }
    });

    button7->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        if (image.IsOk())
        {
            wxImage image2;
            if (pog_mirror == false)
            {
                image2 = image.Mirror(true);
                pog_mirror = true;
            }
            else
            {
                image2 = image.Mirror(false);
                pog_mirror = false;
            }
            image = image2.Copy();
            image2.Clear();
            image2.Destroy();
            reloadImage();
        }
    });
}

void CImageCustomDialog::loadImage()
{
    wxInitAllImageHandlers();

    wxFileDialog openFileDialog(this,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        "jpg and tif files(*.jpg; *.tif) | *.jpg; *.tif", 
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = openFileDialog.GetPath();
        wxImage tmp(path);
        image = tmp;

        wxBitmap bitMap{ wxBitmap(image.Rescale(710, 710, wxIMAGE_QUALITY_HIGH)) };
        picture->SetBitmap(bitMap);
        picture->SetSize(710, 710);
    }
}

void CImageCustomDialog::reloadImage(int factor)
{
    if (image.IsOk() == true)
    {
        wxBitmap bitMap{ wxBitmap(image.Rescale(factor, factor, wxIMAGE_QUALITY_HIGH)) };
        picture->SetBitmap(bitMap);
        picture->SetSize(factor, factor);
    }
}

void CImageCustomDialog::SaveImage()
{
    wxFileDialog saveFileDialog(this,
    wxEmptyString,
    wxEmptyString,
    "MyFile.jpg", "Text Files (*.jpg)|*.jpg|All Files (*.*)|*.*",
    wxFD_SAVE);
    if (saveFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = saveFileDialog.GetPath();
        wxImage tmp;
        tmp = image.Copy();
        tmp.SaveFile(path, wxBITMAP_TYPE_JPEG);
    }
}


CInputDialog::CInputDialog(wxFrame* parent) :CInputDialogBase{ parent,"Basic Algorithms Selection" }
{
    setControlslayout();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        // set values
        int item = comboBox1->GetSelection();
        SelectionText = comboBox1->GetValue();
        DoFunction();
        Close();
    });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        // cancel
        Close();
    });
}

CGridInputDialog::CGridInputDialog(wxFrame* parent):CInputDialogBase{ parent,"Custom Kernel Input"}
{
    setControlslayout();

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

    button4->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
    {
        wxFileDialog saveFileDialog(    this, 
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
        wxFileDialog openFileDialog(    this,
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
            LoadDataToFile(obs, grid);
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
                Mat out = ApplyCustom2Dfilter(img_orig, kernel);

                if (out.empty() == false)
                {
                    imghelper->setFinalImageOpenCV(out);
                    showImage(out, "Final");
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