#include "childframes.h"
#include "savekernel.h"

CInputDialogBase::CInputDialogBase(wxFrame* parent, wxString name) :wxFrame{ parent, -1, name, wxPoint(-1, -1) }
{
}

std::string CInputDialogBase::setPath(bool Gray)
{
    auto spath = imghelper->getOriginalImage();    
    imghelper->setFinalGray(Gray);
    return spath;
}

void  CInputDialogBase::setFinalImg(Mat& out)
{
    if (out.empty() == false)
    {
        imghelper->setFinalImageOpenCV(out);
        outxt->writeTo("Algorithm applied correctly\n");
        imghelper->SetOriginalNew();
    }
    else
    {
        outxt->writeTo("Algorithm error\n");
    }
}

void
CInputDialogBase::ApplyAlgorithm(Function1Parameter& f, bool Gray)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img);
            setFinalImg(out);
            imghelper->addImageToCache(out);
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV());
        setFinalImg(out);
        imghelper->addImageToCache(out);
    }
}

void
CInputDialogBase::ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img, kernel_size);
            setFinalImg(out);
            imghelper->addImageToCache(out);
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), kernel_size);
        setFinalImg(out);
        imghelper->addImageToCache(out);
    }
}

