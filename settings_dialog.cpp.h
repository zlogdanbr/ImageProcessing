#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
#include <sstream>
#include <string>

struct SobelSettings
{
	int depth = 1;
	int type = 1;
	double delta = 10.0;
};

struct LaplacianSettings
{
	int scale = 1;
	int delta = 1;
	int ddepth = 1;
};

struct CannySettings
{
	int lowthreshold = 125;
	int highthreshold = 350;
};

struct GaussianSettings
{
	double sigmaX = 1.0;
	double sigmaY = 1.0;
};

struct Settings
{
	SobelSettings  sobel;
	LaplacianSettings	laplacian;
	CannySettings	canny;
	GaussianSettings gaussian;

	static double convertWxStringToDouble(wxString _s)
	{
		std::stringstream os;
		os << _s;
		return std::stod(os.str());
	}
		
	static int convertWxStringToInt(wxString _s)
	{
		std::stringstream os;
		os << _s;
		return std::stoi(os.str());
	}

	template<typename T>
	static wxString convertDoubleToWxString(T t)
	{
		std::stringstream os;
		os << t;
		return os.str().c_str();
	}

	static std::string convertWxStringToString(wxString _s)
	{
		std::stringstream os;
		os << _s;
		return os.str();
	}

	bool empty = true;

	Settings clone()
	{
		Settings _settings;
		_settings.canny.highthreshold = this->canny.highthreshold;
		_settings.canny.lowthreshold = this->canny.lowthreshold;

		_settings.laplacian.ddepth = this->laplacian.ddepth;
		_settings.laplacian.delta = this->laplacian.delta;
		_settings.laplacian.scale = this->laplacian.scale;

		_settings.gaussian.sigmaX = this->gaussian.sigmaX;
		_settings.gaussian.sigmaY = this->gaussian.sigmaY;

		_settings.sobel.delta = this->sobel.delta;
		_settings.sobel.depth = this->sobel.depth;
		_settings.sobel.type = this->sobel.type;

		_settings.empty = this->empty;

		return _settings;
	}
};

class CSettingsDialog final : public wxDialog
{
private:

	int getValueInt(wxTextCtrl* txtcrtl)
	{
		return Settings::convertWxStringToInt(txtcrtl->GetValue());
	}

	double getValueDouble(wxTextCtrl* txtcrtl)
	{
		return Settings::convertWxStringToDouble(txtcrtl->GetValue());
	}

	void setValueInt(int v, wxTextCtrl* txtcrtl)
	{
		wxString s  = Settings::convertDoubleToWxString<int>(v);
		txtcrtl->SetValue(s);
	}

	void setValueDouble(double v, wxTextCtrl* txtcrtl)
	{
		wxString s = Settings::convertDoubleToWxString<double>(v);
		txtcrtl->SetValue(s);
	}

protected:
	wxButton* m_button6;
	wxButton* m_button7;
	wxButton* m_button8;
	wxStaticText* m_staticText18;
	wxTextCtrl* m_textCtrl16;
	wxStaticText* m_staticText19;
	wxTextCtrl* m_textCtrl17;
	wxStaticText* m_staticText20;
	wxTextCtrl* m_textCtrl18;
	wxStaticText* m_staticText21;
	wxTextCtrl* m_textCtrl19;
	wxStaticText* m_staticText22;
	wxTextCtrl* m_textCtrl20;
	wxStaticText* m_staticText23;
	wxTextCtrl* m_textCtrl21;
	wxStaticText* m_staticText24;
	wxTextCtrl* m_textCtrl22;
	wxStaticText* m_staticText25;
	wxTextCtrl* m_textCtrl23;
	wxStaticText* m_staticText26;
	wxTextCtrl* m_textCtrl24;
	wxStaticText* m_staticText27;
	wxTextCtrl* m_textCtrl25;

	Settings _settings;


public:

	CSettingsDialog(	wxWindow* parent,
				wxWindowID id = wxID_ANY,
				const wxString& title = wxT("Settings"),
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxSize(559, 473),
				long style = wxCAPTION | wxDEFAULT_DIALOG_STYLE);

	~CSettingsDialog();

	Settings getSettings() { return _settings; };
	void setSettings(Settings& s) 
	{ 
		_settings = s.clone();
	};
	void setValuesInTheUI();
	void initSettings();
	void getValuesFromUI();

};
