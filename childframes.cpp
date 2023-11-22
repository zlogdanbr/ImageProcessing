#include "childframes.h"
#include "savekernel.h"

CInputDialogBase::CInputDialogBase(wxFrame* parent, wxString name) :wxFrame{ parent, -1, name, wxPoint(-1, -1) }
{
}

void
CInputDialogBase::ApplyAlgorithm(Function1Parameter& f, bool Gray)
{

    if (imghelper->getOriginalImageInitiated() == false)
    {
        auto spath = imghelper->getOriginalImage();
        Mat img;
        Mat out;
        imghelper->setFinalGray(Gray);

        if (loadImage(spath, img) == true)
        {
            out = f(img);
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
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV());
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
}

void
CInputDialogBase::ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        auto spath = imghelper->getOriginalImage();
        Mat img;
        Mat out;
        imghelper->setFinalGray(Gray);

        if (loadImage(spath, img) == true)
        {
            out = f(img, kernel_size);
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
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), kernel_size);
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
}
