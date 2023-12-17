#include "settings_dialog.cpp.h"

CSettingsDialog::CSettingsDialog(	wxWindow* parent, 
						wxWindowID id, 
						const wxString& title, 
						const wxPoint& pos, 
						const wxSize& size, 
						long style) : 
						wxDialog(parent, id, title, pos, size, style)
{

	initSettings();

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	
	//---------------------------------------------------------------------
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Settings")), wxHORIZONTAL);

	m_button6 = new wxButton(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer1->Add(m_button6, 0, wxALL, 5);

	m_button7 = new wxButton(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer1->Add(m_button7, 0, wxALL, 5);

	m_button8 = new wxButton(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer1->Add(m_button8, 0, wxALL, 5);

	bSizer1->Add(sbSizer1, 1, wxEXPAND, 5);

	//---------------------------------------------------------------------
	//	sobel
	// m_textCtrl16		Depth
	// m_textCtrl17		Type
	// m_textCtrl18		Delta
	//---------------------------------------------------------------------
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Sobel")), wxHORIZONTAL);
	m_staticText18 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Depth"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText18->Wrap(-1);
	sbSizer2->Add(m_staticText18, 0, wxALL, 5);
	m_textCtrl16 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer2->Add(m_textCtrl16, 0, wxALL, 5);
	m_staticText19 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText19->Wrap(-1);
	sbSizer2->Add(m_staticText19, 0, wxALL, 5);
	m_textCtrl17 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer2->Add(m_textCtrl17, 0, wxALL, 5);
	m_staticText20 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Delta"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText20->Wrap(-1);
	sbSizer2->Add(m_staticText20, 0, wxALL, 5);
	m_textCtrl18 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer2->Add(m_textCtrl18, 0, wxALL, 5);
	bSizer1->Add(sbSizer2, 1, wxEXPAND, 5);

	//---------------------------------------------------------------------
	//Laplacian
	// m_textCtrl19	Scale
	// m_textCtrl20	Delta
	// m_textCtrl21 Depth
	//---------------------------------------------------------------------
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Laplacian")), wxHORIZONTAL);
	m_staticText21 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21->Wrap(-1);
	sbSizer3->Add(m_staticText21, 0, wxALL, 5);
	m_textCtrl19 = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer3->Add(m_textCtrl19, 0, wxALL, 5);
	m_staticText22 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Delta"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText22->Wrap(-1);
	sbSizer3->Add(m_staticText22, 0, wxALL, 5);
	m_textCtrl20 = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer3->Add(m_textCtrl20, 0, wxALL, 5);
	m_staticText23 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Depth"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText23->Wrap(-1);
	sbSizer3->Add(m_staticText23, 0, wxALL, 5);
	m_textCtrl21 = new wxTextCtrl(sbSizer3->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer3->Add(m_textCtrl21, 0, wxALL, 5);
	bSizer1->Add(sbSizer3, 1, wxEXPAND, 5);

	//---------------------------------------------------------------------
	// Canny
	// m_textCtrl22 Low Threshold
	// m_textCtrl23 High Threshold
	//---------------------------------------------------------------------
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Canny")), wxHORIZONTAL);
	m_staticText24 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, wxT("Low Threshold"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText24->Wrap(-1);
	sbSizer4->Add(m_staticText24, 0, wxALL, 5);
	m_textCtrl22 = new wxTextCtrl(sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer4->Add(m_textCtrl22, 0, wxALL, 5);
	m_staticText25 = new wxStaticText(sbSizer4->GetStaticBox(), wxID_ANY, wxT("High Threshold"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText25->Wrap(-1);
	sbSizer4->Add(m_staticText25, 0, wxALL, 5);
	m_textCtrl23 = new wxTextCtrl(sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer4->Add(m_textCtrl23, 0, wxALL, 5);
	bSizer1->Add(sbSizer4, 1, wxEXPAND, 5);

	//---------------------------------------------------------------------
	// Gaussian
	// m_textCtrl24 Sigma X
	// m_textCtrl25 Sigma Y
	//---------------------------------------------------------------------
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Gaussian")), wxHORIZONTAL);
	m_staticText26 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY, wxT("Sigma X"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText26->Wrap(-1);
	sbSizer5->Add(m_staticText26, 0, wxALL, 5);
	m_textCtrl24 = new wxTextCtrl(sbSizer5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer5->Add(m_textCtrl24, 0, wxALL, 5);
	m_staticText27 = new wxStaticText(sbSizer5->GetStaticBox(), wxID_ANY, wxT("Sigma Y"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText27->Wrap(-1);
	sbSizer5->Add(m_staticText27, 0, wxALL, 5);
	m_textCtrl25 = new wxTextCtrl(sbSizer5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	sbSizer5->Add(m_textCtrl25, 0, wxALL, 5);
	bSizer1->Add(sbSizer5, 1, wxEXPAND, 5);

	//---------------------------------------------------------------------
	this->SetSizer(bSizer1);
	this->Layout();
	this->Centre(wxBOTH);

	//---------------------------------------------------------------------

	m_button6->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			// Save
			getValuesFromUI();
			_settings.empty = false;
		});

	m_button7->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			// Clear
		});

	m_button8->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			// Close
			Close();
		});

}

CSettingsDialog::~CSettingsDialog()
{
}

void CSettingsDialog::initSettings()
{
	_settings.canny.highthreshold = 150;
	_settings.canny.lowthreshold = 320;

	_settings.laplacian.ddepth = 10;
	_settings.laplacian.delta = 10.0;
	_settings.laplacian.scale = 1;

	_settings.gaussian.sigmaX = 1;
	_settings.gaussian.sigmaY = 1;

	_settings.sobel.delta = 1;
	_settings.sobel.depth = 1;
	_settings.sobel.type = 1;

}

void CSettingsDialog::setValuesInTheUI()
{
	if (_settings.empty == false)
	{
		//---------------------------------------------------------------------
		//	sobel
		// m_textCtrl16		Depth
		// m_textCtrl17		Type
		// m_textCtrl18		Delta
		//---------------------------------------------------------------------
		setValueInt(_settings.sobel.depth, m_textCtrl16);
		setValueInt(_settings.sobel.type, m_textCtrl17);
		setValueDouble(_settings.sobel.delta, m_textCtrl18);
		//---------------------------------------------------------------------
		//Laplacian
		// m_textCtrl19	Scale
		// m_textCtrl20	Delta
		// m_textCtrl21 Depth
		//---------------------------------------------------------------------
		setValueInt(_settings.laplacian.scale, m_textCtrl19);
		setValueDouble(_settings.laplacian.delta, m_textCtrl20);
		setValueInt(_settings.laplacian.ddepth, m_textCtrl21);
		//---------------------------------------------------------------------
		// Canny
		// m_textCtrl22 Low Threshold
		// m_textCtrl23 High Threshold
		//---------------------------------------------------------------------
		setValueInt(_settings.canny.lowthreshold, m_textCtrl22);
		setValueInt(_settings.canny.lowthreshold, m_textCtrl23);
		//---------------------------------------------------------------------
		// Gaussian
		// m_textCtrl24 Sigma X
		// m_textCtrl25 Sigma Y
		//---------------------------------------------------------------------
		setValueDouble(_settings.gaussian.sigmaX, m_textCtrl24);
		setValueDouble(_settings.gaussian.sigmaY, m_textCtrl25);

	}
	else
	{
		_settings.empty = false;
		setValuesInTheUI();
	}

}

void CSettingsDialog::getValuesFromUI()
{

	//---------------------------------------------------------------------
	//	sobel
	// m_textCtrl16		Depth
	// m_textCtrl17		Type
	// m_textCtrl18		Delta
	//---------------------------------------------------------------------
	_settings.sobel.depth = getValueInt(m_textCtrl16);
	_settings.sobel.type = getValueInt(m_textCtrl17);
	_settings.sobel.delta = getValueDouble(m_textCtrl18);
	//---------------------------------------------------------------------
	//Laplacian
	// m_textCtrl19	Scale
	// m_textCtrl20	Delta
	// m_textCtrl21 Depth
	//---------------------------------------------------------------------
	_settings.laplacian.scale = getValueInt(m_textCtrl19);
	_settings.laplacian.delta = getValueInt(m_textCtrl20);
	_settings.laplacian.ddepth = getValueDouble(m_textCtrl21);
	//---------------------------------------------------------------------
	// Canny
	// m_textCtrl22 Low Threshold
	// m_textCtrl23 High Threshold
	//---------------------------------------------------------------------
	_settings.canny.lowthreshold = getValueInt(m_textCtrl22);
	_settings.canny.highthreshold = getValueInt(m_textCtrl23);
	//---------------------------------------------------------------------
	// Gaussian
	// m_textCtrl24 Sigma X
	// m_textCtrl25 Sigma Y
	//---------------------------------------------------------------------
	_settings.gaussian.sigmaX = getValueDouble(m_textCtrl24);
	_settings.gaussian.sigmaY = getValueDouble(m_textCtrl25);

	_settings.empty = false;

}
