#include "childframes.h"

CLoadImageSet::CLoadImageSet(	wxWindow* parent,
								CWriteLogs* outxt,
								wxWindowID id,
								const wxString& title,
								const wxPoint& pos,
								const wxSize& size, long style) :
								outxt{ outxt },
								wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	m_panel5 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20), wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer(wxHORIZONTAL);

	m_buttonOK = new wxButton(m_panel5, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer13->Add(m_buttonOK, 0, wxALL, 5);

	m_buttonClear = new wxButton(m_panel5, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer13->Add(m_buttonClear, 0, wxALL, 5);

	m_panel5->SetSizer(bSizer13);
	m_panel5->Layout();
	bSizer10->Add(m_panel5, 1, wxEXPAND | wxALL, 5);

	m_panel6 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer(wxVERTICAL);

	m_buttonLoadImage1 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage1, 0, wxALL, 5);

	m_buttonLoadImage2 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage2, 0, wxALL, 5);

	m_buttonLoadImage3 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage3, 0, wxALL, 5);

	m_buttonLoadImage4 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage4, 0, wxALL, 5);

	m_buttonLoadImage5 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage5, 0, wxALL, 5);

	m_buttonLoadImage6 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage6, 0, wxALL, 5);

	m_buttonLoadImage7 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage7, 0, wxALL, 5);

	m_buttonLoadImage8 = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer15->Add(m_buttonLoadImage8, 0, wxALL, 5);


	bSizer14->Add(bSizer15, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);

	m_textCtrlImagePath1 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath1, 0, wxALL, 5);

	m_textCtrlImagePath2 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath2, 0, wxALL, 5);

	m_textCtrlImagePath3 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath3, 0, wxALL, 5);

	m_textCtrlImagePath4 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath4, 0, wxALL, 5);

	m_textCtrlImagePath5 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath5, 0, wxALL, 5);

	m_textCtrlImagePath6 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath6, 0, wxALL, 5);

	m_textCtrlImagePath7 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath7, 0, wxALL, 5);

	m_textCtrlImagePath8 = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
	bSizer17->Add(m_textCtrlImagePath8, 0, wxALL, 5);


	bSizer14->Add(bSizer17, 1, wxEXPAND, 5);


	m_panel6->SetSizer(bSizer14);
	m_panel6->Layout();
	bSizer14->Fit(m_panel6);
	bSizer10->Add(m_panel6, 1, wxEXPAND | wxALL, 5);

	this->SetSizer(bSizer10);
	this->Layout();

	this->Centre(wxBOTH);

	// add code from here
	setEventButtons();

}

CLoadImageSet::~CLoadImageSet()
{
}

bool CLoadImageSet::readImagePath(wxWindow* parent, wxTextCtrl* txtctrl)
{
	wxFileDialog openFileDialog(parent,
		wxEmptyString,
		wxEmptyString,
		wxEmptyString,
		"jpg and tif files(*.jpg; *.tif)| *.jpg; *.tif|All Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	Mat img1;
	openFileDialog.SetFilterIndex(0);
	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxString path = openFileDialog.GetPath();
		std::string spath = convertWxStringToString(path);
		setTextAtBox(txtctrl, path);
		return true;
	}
	return false;
}

void CLoadImageSet::setEventButtons()
{
	
	_paths = {	m_textCtrlImagePath1 ,
				m_textCtrlImagePath2 ,
				m_textCtrlImagePath3 ,
				m_textCtrlImagePath4 ,
				m_textCtrlImagePath5 ,
				m_textCtrlImagePath6 ,
				m_textCtrlImagePath7 ,
				m_textCtrlImagePath8 };

	_actions = {	m_buttonLoadImage1 ,
					m_buttonLoadImage2 ,
					m_buttonLoadImage3 ,
					m_buttonLoadImage4 ,
					m_buttonLoadImage5 ,
					m_buttonLoadImage6 ,
					m_buttonLoadImage7 ,
					m_buttonLoadImage8 };

	m_buttonOK->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			IsoK = true;
			Close();
		});

	_actions[0]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[0]);
		});

	_actions[1]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[1]);
		});

	_actions[2]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[2]);
		});

	_actions[3]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[3]);
		});

	_actions[4]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[4]);
		});

	_actions[5]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[5]);
		});

	_actions[6]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[6]);
		});

	_actions[7]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[7]);
		});


}

void CLoadImageSet::setImageArray()
{
	for (  auto& txtcrl : _paths)
	{
		wxString path = getTextFromBox(txtcrl);
		std::string spath = convertWxStringToString(path);
		if (spath != "")
		{
			Mat img;
			if (loadImage(spath, img) == true)
			{
				_images.push_back(img);
			}
		}
	}

}

void CLoadImageSet::doProcess()
{

	setImageArray();

	if (_images.size() < 2)
	{
		return;
	}
	ApplyAndCompare(_images);

}

