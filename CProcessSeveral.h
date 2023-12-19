#pragma once

#include "image_util.h"
#include <wx/dirdlg.h>
#include <wx/stdpaths.h>
#include <wx/windowptr.h>

class CProcessSeveral
{
public:
	CProcessSeveral() = default;
	~CProcessSeveral() noexcept = default;

	bool readImagesToInternal(wxWindow* parent);
	void doProcess();

private:

	bool addImageToInternal(Mat& img, wxWindow* parent);
	std::vector<Mat> _images;

};

