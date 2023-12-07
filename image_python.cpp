#include "childframes.h"

CPythonDialog::CPythonDialog(wxFrame* parent):CInputDialogBase { parent, "Pyton scripts" }
{
    this->SetSize(410, 103);
    setControlslayout();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
                wxFileDialog openFileDialog(this,
                wxEmptyString,
                wxEmptyString,
                wxEmptyString,
                "python files(*.py) | *.py",
                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

                imghelper->clean();
                openFileDialog.SetFilterIndex(0);
                if (openFileDialog.ShowModal() == wxID_OK)
                {
                    wxString path = openFileDialog.GetPath();
                    std::string spath = convertWxStringToString(path);
                    textCtrl1->Clear();
                    textCtrl1->AppendText(path);
                }
        });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {

        });
}

void CPythonDialog::setControlslayout()
{
    // set base sizer
    
    basePanel->SetSize(400, 50);
    panel1->SetSize(380, 30);
    panel2->SetSize(380, 30);
    basePanel->SetSizer(baseSizer);

    // add buttons to the horizontal box
    hbox1->Add(button1);
    hbox1->Add(button2);

    // add buttons to the horizontal box
    hbox2->Add(textCtrl1);

    // set horizontal base sizer at panel1 and panel2
    panel1->SetSizer(hbox1);
    panel2->SetSizer(hbox2);

    // add panel1 to the base sizer at the base panel
    baseSizer->Add(panel1);
    baseSizer->Add(panel2);


    Center();
}