#include "childframes.h"

CSliderDialog::CSliderDialog(   wxWindow* parent, 
                        info& inf,
                        wxWindowID id, 
                        const wxString& title, 
                        const wxPoint& pos, 
                        const wxSize& size, 
                        long style
                        ) : wxDialog(parent, id, inf.title, pos, size, style)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer9;
    bSizer9 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer10;
    bSizer10 = new wxBoxSizer(wxHORIZONTAL);

    m_button5 = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer10->Add(m_button5, 0, wxALL, 5);


    bSizer9->Add(bSizer10, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer(wxHORIZONTAL);

    m_slider5 = new wxSlider(this, wxID_ANY, inf.default_value, inf.min, inf.max, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    bSizer11->Add(m_slider5, 0, wxALL, 5);

    bSizer9->Add(bSizer11, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer12;
    bSizer12 = new wxBoxSizer(wxHORIZONTAL);

    m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText3->Wrap(-1);
    bSizer12->Add(m_staticText3, 0, wxALL, 5);


    bSizer9->Add(bSizer12, 1, wxEXPAND, 5);


    this->SetSizer(bSizer9);
    this->Layout();

    this->Centre(wxBOTH);

    m_button5->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            Close();
        });

    m_slider5->Bind(wxEVT_SLIDER, [&](wxCommandEvent& event)
        {
            const int fill = m_slider5->GetValue();
            threshold_value = static_cast<double>(fill);
            std::stringstream os;
            os << fill;
            m_staticText3->SetLabel(os.str().c_str());
            Refresh();
        });

}

CSliderDialog::~CSliderDialog()
{
}