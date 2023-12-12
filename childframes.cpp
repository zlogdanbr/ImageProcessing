#include "childframes.h"
#include "savekernel.h"

CInputDialogBase::CInputDialogBase(wxFrame* parent, wxString name) :wxFrame{ parent, -1, name, wxPoint(-1, -1) }
{
}

std::string 
CInputDialogBase::setPath(bool Gray)
{
    auto spath = imghelper->getOriginalImage();    
    imghelper->setFinalGray(Gray);
    return spath;
}

void  
CInputDialogBase::setFinalImg(Mat& out)
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
CInputDialogBase::ApplyAlgorithm(   Function1Parameter& f, 
                                    bool Gray)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img);
            auto s = out.size();

            if (s.height == 0 || s.width == 0)
            {
                return;
            }
            setFinalImg(out);
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
    }
}

void
CInputDialogBase::ApplyAlgorithm(   Function4Parameters& f,
                                    bool Gray,
                                    int kernel_size,
                                    double p1,
                                    double p2)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img, kernel_size, p1, p2);
            setFinalImg(out);
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), kernel_size, p1, p2);
        setFinalImg(out);
    }
}

void 
CInputDialogBase::ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img, p1, p2);
            setFinalImg(out);
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), p1, p2);
        setFinalImg(out);
    }
}

void
CInputDialogBase::ApplyAlgorithm(   Function5Parameters& f, 
                                    bool Gray, 
                                    int kernel_size, 
                                    int p1, 
                                    int p2, 
                                    int p3)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img, kernel_size, p1, p2, p3);
            setFinalImg(out);
        }
        else
        {
            outxt->writeTo("Image not loaded\n");
        }
    }
    else
    {
        Mat out;
        out = f(imghelper->getOrginalImageOpenCV(), kernel_size, p1, p2, p3);
        setFinalImg(out);
    }

}

