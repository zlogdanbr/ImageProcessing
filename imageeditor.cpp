#include "childframes.h"
#include "savekernel.h"

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
                    auto hn = static_cast<int>(h / scale);
                    auto wn = static_cast<int>(w / scale);

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
                    auto _h = image.GetHeight();
                    auto _w = image.GetWidth();
                    auto hn = static_cast<int>(h * scale);
                    auto wn = static_cast<int>(w * scale);

                    if (hn <= 0 || hn > 2000)
                    {
                        hn = _h;
                    }

                    if (wn <= 0 || wn > 2000)
                    {
                        wn = _w;
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
            if (image.IsOk())
            {
                wxImage image2 = image.Rotate90(true);
                image = image2.Copy();
                image2.Clear();
                image2.Destroy();
                reloadImage(-1);
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
                reloadImage(-1);
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

        w = image.GetWidth();
        h = image.GetHeight();

        wxBitmap bitMap{ wxBitmap(image.Rescale(w, h, wxIMAGE_QUALITY_HIGH)) };
        picture->SetBitmap(bitMap);
        picture->SetSize(w, h);
    }
}

void CImageCustomDialog::reloadImage(int factor)
{
    if (image.IsOk() == true)
    {
        if (factor > 0)
        {
            wxBitmap bitMap{ wxBitmap(image.Rescale(factor, factor, wxIMAGE_QUALITY_HIGH)) };
            picture->SetBitmap(bitMap);
            picture->SetSize(factor, factor);
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
