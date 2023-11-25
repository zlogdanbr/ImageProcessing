#include "childframes.h"

void CImageCustomDialog::setControlslayout()
{
    // set base sizer
    this->SetSize(710, 710);
    basePanel->SetSize(710, 710);
    basePanel->SetSizer(baseSizer);

    // add buttons to the horizontal box
    vbox1->Add(button5);
    vbox1->Add(button2);
    vbox1->Add(button3);
    vbox1->Add(button6);
    vbox1->Add(button4);
    vbox1->Add(button8);
    vbox1->Add(button9);

    vbox2->Add(picture, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);// wxALIGN_CENTER_HORIZONTAL

    // set horizontal base sizer at panel1 and panel2
    panel1->SetSizer(vbox1);
    panel2->SetSizer(vbox2);

    // add panel1 to the base sizer at the base panel
    baseSizer->Add(panel1);
    baseSizer->Add(panel2);

    Center();
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
                    auto _h = image.GetHeight();
                    auto _w = image.GetWidth();
                    auto hn = static_cast<int>(_h / scale);
                    auto wn = static_cast<int>(_w / scale);

                    if (hn <= 0)
                    {
                        hn = _h;
                    }

                    if (wn <= 0)
                    {
                        wn = _w;
                    }

                    wxImage image2 = image.Scale(hn, wn);
                    image = image2.Copy();
                    image2.Clear();
                    reloadImage(hn,wn);
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
                    auto _h = image.GetHeight();
                    auto _w = image.GetWidth();
                    auto hn = static_cast<int>(_h * scale);
                    auto wn = static_cast<int>(_w * scale);

                    wxImage image2 = image.Scale(hn, wn);
                    image = image2.Copy();
                    image2.Clear();
                    reloadImage(hn, wn);
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
            if (image.IsOk())
            {
                wxImage image2 = image.Rotate90(true);
                image = image2.Copy();
                image2.Clear();
                image2.Destroy();
                reloadImage(-1,-1);
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
                reloadImage(-1,-1);
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

        w = image.GetWidth();
        h = image.GetHeight();

        wxBitmap bitMap{ image };
        picture->SetBitmap(bitMap);
        picture->SetSize(w, h);
    }
}

void CImageCustomDialog::reloadImage(int factor1,int factor2)
{
    if (image.IsOk() == true)
    {
        if (factor1 > 0 && factor2 > 0)
        {
            wxBitmap bitMap{ wxBitmap(image.Rescale(factor1, factor2, wxIMAGE_QUALITY_HIGH)) };
            picture->SetBitmap(bitMap);
            picture->SetSize(factor1, factor2);
            return;
        }

        wxBitmap bitMap{ wxBitmap(image) };
        picture->SetBitmap(bitMap);

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
