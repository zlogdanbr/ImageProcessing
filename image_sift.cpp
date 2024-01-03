#include "childframes.h"
#include "image_interest_points.h"

CLoadImageSetBase::CLoadImageSetBase(	wxWindow* parent,
										CWriteLogs* outxt,
										wxWindowID id,
										const wxString& title,
										int inputs,
										const wxPoint& pos,
										const wxSize& size, long style) :
										outxt{ outxt },
										wxDialog(parent, id, title, pos, size, style),
										inputs{ inputs }
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
	AddButtons(bSizer15);

	bSizer14->Add(bSizer15, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);
	AddTextCrlt(bSizer17);

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

CLoadImageSetBase::~CLoadImageSetBase()
{
}


void CLoadImageSetBase::AddButtons(wxBoxSizer* sizer1)
{
	for (int i = 0; i < inputs; i++)
	{
		_actions[i] = new wxButton(m_panel6, wxID_ANY, wxT("Load Image"), wxDefaultPosition, wxDefaultSize, 0);
		sizer1->Add(_actions[i], 0, wxALL, 5);
	}
}

void CLoadImageSetBase::AddTextCrlt(wxBoxSizer* sizer2)
{
	for (int i = 0; i < inputs; i++)
	{
		_paths[i] = new wxTextCtrl(m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0);
		sizer2->Add(_paths[i], 0, wxALL, 5);
	}
}

bool CLoadImageSetBase::readImagePath(wxWindow* parent, wxTextCtrl* txtctrl)
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
		_filenames.push_back(spath);
		return true;
	}
	return false;
}

void CLoadImageSetBase::setEventButtons()
{
	
	m_buttonOK->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			IsoK = true;
			Close();
		});

	int i = 0;
	_actions[0]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[0]);
		});
	i++;

	_actions[1]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[1]);
		});

	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[2]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[2]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[3]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[3]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[4]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[4]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[5]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[5]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[6]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[6]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}

	_actions[7]->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
		{
			readImagePath(this, _paths[7]);
		});
	i++;
	if (i == inputs)
	{
		return;
	}
}

void CLoadImageSetBase::setImageArray()
{
	for (int i = 0 ; i < inputs; i++)
	{
		wxString path = getTextFromBox(_paths[i]);
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

CApplySift::CApplySift(	wxWindow* parent,
						CWriteLogs* outxt,
						wxWindowID id,
						const wxString& title,
						int inputs)
					    :CLoadImageSetBase(parent, outxt, wxID_ANY, title, inputs)
{

}

void CApplySift::doProcess()
{
	setImageArray();
	if (_images.size() < 2)
	{
		return;
	}

	try
	{
		sift_algo::ApplyAndCompareSIFT(_images, _filenames);
	}
	catch (std::exception& e)
	{
		std::string a = e.what();
		wxMessageBox(a.c_str(), "Error", wxOK | wxICON_ERROR);
	}
}

