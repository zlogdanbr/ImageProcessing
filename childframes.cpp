#include "childframes.h"
#include "savekernel.h"

CInputDialogBase::CInputDialogBase(wxFrame* parent, wxString name) :wxFrame{ parent, -1, name, wxPoint(-1, -1) }
{
    _parent = parent;
}

std::string 
CInputDialogBase::setPath(bool Gray)
{
    auto spath = imghelper->getOriginalImage();    
    imghelper->setFinalGray(Gray);
    return spath;
}

Mat  
CInputDialogBase::setFinalImg(Mat& out)
{
    Mat _out;
    if (out.empty() == false)
    {
        imghelper->setFinalImageOpenCV(out);
        outxt->writeTo("Algorithm applied correctly\n");
        _out = imghelper->SetOriginalNew();
    }
    else
    {
        outxt->writeTo("Algorithm error\n");
    }
    return _out;
}

