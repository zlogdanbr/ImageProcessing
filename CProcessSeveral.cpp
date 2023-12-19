#include "CProcessSeveral.h"
#include "childframes.h"


bool CProcessSeveral::addImageToInternal(Mat& img, wxWindow* parent)
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

		if (loadImage(spath, img1) == true)
		{
			_images.push_back(img1);
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool CProcessSeveral::readImagesToInternal(wxWindow* parent)
{
	Mat img1;
	Mat img2;

	if (addImageToInternal(img1, parent) == false)
	{
		return false;
	}

	if (addImageToInternal(img2, parent) == false)
	{
		return false;
	}

	return true;
}

void CProcessSeveral::doProcess()
{
	if (_images.size() < 2)
	{
		return;
	}

	applyMultiple(_images);

}
