#include "childframes.h"

CSelectTemplateParam::CSelectTemplateParam(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxHORIZONTAL);

	m_button5 = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer7->Add(m_button5, 0, wxALL, 5);

	m_button6 = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer7->Add(m_button6, 0, wxALL, 5);


	bSizer6->Add(bSizer7, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxVERTICAL);

	m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("Select Mode"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer9->Add(m_staticText5, 0, wxALL, 5);

	m_staticText6 = new wxStaticText(this, wxID_ANY, wxT("MIN:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	bSizer9->Add(m_staticText6, 0, wxALL, 5);

	m_staticText7 = new wxStaticText(this, wxID_ANY, wxT("MAX:"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText7->Wrap(-1);
	bSizer9->Add(m_staticText7, 0, wxALL, 5);


	bSizer8->Add(bSizer9, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	m_comboBox3 = new wxComboBox(this, wxID_ANY, wxT("TM_SQDIFF"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	m_comboBox3->Append("TM_SQDIFF");
	m_comboBox3->Append("TM_SQDIFF_NORMED");
	m_comboBox3->Append("TM_CCORR");
	m_comboBox3->Append("TM_CCORR_NORMED");
	m_comboBox3->Append("TM_CCOEFF");
	m_comboBox3->Append("TM_CCOEFF_NORMED");
	bSizer10->Add(m_comboBox3, 0, wxALL, 5);

	m_spinCtrlDouble3 = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 125, 1);
	m_spinCtrlDouble3->SetDigits(0);
	bSizer10->Add(m_spinCtrlDouble3, 0, wxALL, 5);

	m_spinCtrlDouble4 = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 350, 1);
	m_spinCtrlDouble4->SetDigits(0);
	bSizer10->Add(m_spinCtrlDouble4, 0, wxALL, 5);


	bSizer8->Add(bSizer10, 1, wxEXPAND, 5);


	bSizer6->Add(bSizer8, 1, wxEXPAND, 5);


	bSizer5->Add(bSizer6, 1, wxEXPAND, 5);


	this->SetSizer(bSizer5);
	this->Layout();

	this->Centre(wxBOTH);

	m_button5->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			IsOK = true;
			Close();
		});

	m_button6->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			wxString mode = "TM_SQDIFF";
			wxString Algorithm = "Canny";
			_inf.t1 = 125;
			_inf.t2 = 350;
			_inf.t1 = m_spinCtrlDouble3->GetValue();
			_inf.t2 = m_spinCtrlDouble4->GetValue();

			_inf.mode = m_comboBox3->GetValue();
		});
}

CSelectTemplateParam::~CSelectTemplateParam()
{
}
