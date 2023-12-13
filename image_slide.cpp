#include "childframes.h"

CSliderDialog::CSliderDialog(wxString _name ):wxDialog()
{
    this->SetSize(160, 103);
    setControlslayout();
    this->SetTitle(_name);    

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            Close();
        });

    slider->Bind(wxEVT_SLIDER, [&](wxCommandEvent& event)
        {
            const int fill = slider->GetValue();
            threshold_value = static_cast<double>(fill);
            std::stringstream os;
            os << fill;
            staticText->SetLabel(os.str().c_str());
            Refresh();
        });
}

void CSliderDialog::setControlslayout()
{
    // set base sizer
    
    basePanel->SetSize(170, 50);
    panel1->SetSize(150, 30);
    panel2->SetSize(150, 30);
    basePanel->SetSizer(baseSizer);

    // add buttons to the horizontal box
    hbox1->Add(button1);

    hbox2->Add(slider);
    hbox2->Add(staticText);

    // set horizontal base sizer at panel1 and panel2
    panel1->SetSizer(hbox1);
    panel2->SetSizer(hbox2);

    // add panel1 to the base sizer at the base panel
    baseSizer->Add(panel1);
    baseSizer->Add(panel2);


    Center();
}