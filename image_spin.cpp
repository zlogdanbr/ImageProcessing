#include "childframes.h"

CNumentryDouble::CNumentryDouble(	wxWindow* parent,
						double max,
						double min,
						double increment,
						wxWindowID id, 
						const wxString& title, 
						const wxPoint& pos, 
						const wxSize& size, 
						long style) : 
						wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_button2 = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(m_button2, 0, wxALL, 5);

	m_spinCtrlDouble1 = new wxSpinCtrlDouble(this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		min,
		max,
		0,
		increment);
	m_spinCtrlDouble1->SetDigits(0);
	bSizer2->Add(m_spinCtrlDouble1, 0, wxALL, 5);


	this->SetSizer(bSizer2);
	this->Layout();

	this->Centre(wxBOTH);



	m_button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
	{
			// cancel
			Close();
	});
}

CNumentryDouble::~CNumentryDouble()
{
}