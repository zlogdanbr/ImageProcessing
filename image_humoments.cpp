#include "childframes.h"
#include "image_interest_points.h"
#include <iostream>
#include <fstream>

CMatchHuMomments::CMatchHuMomments(wxWindow* parent,
	CWriteLogs* outxt,
	wxWindowID id,
	const wxString& title,
	int inputs)
	:CLoadImageSetBase(parent, outxt, wxID_ANY, title, inputs)
{

}

void CMatchHuMomments::doProcess()
{
	setImageArray();

	wxFileDialog saveFileDialog(this,
		wxEmptyString,
		wxEmptyString,
		"descriptors.csv",
		"Text Files (*.csv)|*.csv|All Files (*.*)|*.*",
		wxFD_SAVE);

	std::string path;
	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxString spath = saveFileDialog.GetPath();
		path = convertWxStringToString(spath);
	}
	else
	{
		return;
	}
	std::ofstream outputFile;
	outputFile.open(path, std::ios::app);
	if (outputFile.is_open())
	{
		for (auto& img : _images)
		{
			outputFile << image_info::getHuhMomentsLine(img);
		}
	}
	outputFile.close();

}