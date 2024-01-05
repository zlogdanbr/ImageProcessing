#include "childframes.h"
#include "image_interest_points.h"
#include <wx/busyinfo.h>


namespace op_busy_local
{
	wxBusyInfo* ProgramBusy()
	{
		std::mutex mtex;
		mtex.lock();
		wxWindowDisabler disableAll;
		wxBusyInfo* wait = new wxBusyInfo("Please wait, working...");
		mtex.unlock();
		return wait;
	}

	void Stop(wxBusyInfo* wait)
	{
		wxYield();
		if (nullptr != wait)
		{
			delete wait;
		}
	}
}

CMatchTemplate::CMatchTemplate(wxWindow* parent,
	CWriteLogs* outxt,
	wxWindowID id,
	const wxString& title,
	int inputs)
	:CLoadImageSetBase(parent, outxt, wxID_ANY, title, inputs)
{

}

void CMatchTemplate::doProcess()
{
	setImageArray();
	if (_images.size() < 2)
	{
		return;
	}

	wxBusyInfo* wait = op_busy_local::ProgramBusy();

	try
	{
		auto r = template_matching::ApplyTemplateMatching(_images[0], _images[1]);
		op_busy_local::Stop(wait);
		showImage(r.first, "Original");
	}
	catch (std::exception& e)
	{
		op_busy_local::Stop(wait);
		std::string a = e.what();
		wxMessageBox(a.c_str(), "Error", wxOK | wxICON_ERROR);
	}

}


CMatchTemplateFull::CMatchTemplateFull(wxWindow* parent,
	CWriteLogs* outxt,
	wxWindowID id,
	const wxString& title,
	int inputs)
	:CLoadImageSetBase(parent, outxt, wxID_ANY, title, inputs)
{

}

void CMatchTemplateFull::doProcess()
{
	setImageArray();

	//wxBusyInfo* wait = op_busy_local::ProgramBusy();

	try
	{
		Mat img = _images[0];
		std::vector<Mat> temps{ _images.begin() + 1, _images.end() };

		_images.clear();

		template_info info;
		CSelectTemplateParam dialog(nullptr, wxID_ANY, "Select Parameters");
		dialog.ShowModal();
		if (dialog.IsOK == true)
		{

			Mat r;

			info = dialog._inf;

			if (info.mode == "TM_SQDIFF")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_SQDIFF(img, temps, info.t1, info.t2);
			}
			else
			if (info.mode == "TM_SQDIFF_NORMED")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_SQDIFF_NORMED(img, temps, info.t1, info.t2);
			}
			else
			if (info.mode == "TM_CCORR")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_CCORR(img, temps, info.t1, info.t2);
			}
			else
			if (info.mode == "TM_CCORR_NORMED")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_CCORR_NORMED(img, temps, info.t1, info.t2);
			}
			else
			if (info.mode == "TM_CCOEFF")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_CCOEFF(img, temps, info.t1, info.t2);
			}
			else
			if (info.mode == "TM_CCOEFF_NORMED")
			{
				r = template_matching::canny_matching::ApplyTemplateMatchingFull_TM_CCOEFF_NORMED(img, temps, info.t1, info.t2);
			}
			
			showImage(r, "Original");
		}
		
		//op_busy_local::Stop(wait);

	}
	catch (std::exception& e)
	{
		//op_busy_local::Stop(wait);
		std::string a = e.what();
		wxMessageBox(a.c_str(), "Error", wxOK | wxICON_ERROR);
	}



}


