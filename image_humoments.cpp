#include "childframes.h"


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

	//wxBusyInfo* wait = op_busy_local::ProgramBusy();


	os << "Hu Moments" << std::endl;
	os << "\tImage" << "\tho" << "\th1" << "\th2" << "\th3" << "\th4" << "\th5" << "\th6" << std::endl;
	int cnt = 1;
	for (const auto& img : _images)
	{
		Mat clone = convertograyScale(img);
		// Calculate Moments 
		Moments moments = cv::moments(clone, false);
		// Calculate Hu Moments 
		double huMoments[7];
		HuMoments(moments, huMoments);

		for (int i = 0; i < 7; i++) 
		{
			huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
		}
		os << "\t" << cnt;
		for (const auto& h : huMoments)
		{
			os << "\t" << h;
		}
		os << std::endl;
		cnt++;
	}



}