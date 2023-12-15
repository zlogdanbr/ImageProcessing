#include "childframes.h"
#include <chrono>
#include <ctime>

using namespace std::chrono;
using   tp = high_resolution_clock::time_point;

// starts computing the execution time of a code
const tp
start_tracking()
{
    return high_resolution_clock::now();
}

// stops  computing the execution time of a code and print 
// elapsed time
std::string
end_tracking(tp& start)
{
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::stringstream os;

    os << "Execution time(ms): " << duration.count() / 1000 << std::endl;
    return os.str();
}

CInputDialog::CInputDialog(   wxWindow* parent,
                        CImageHelper* imghelper,
                        CWriteLogs* outxt,
                        wxWindowID id, 
                        const wxString& title, 
                        const wxPoint& pos, 
                        const wxSize& size, 
                        long style) : 
wxDialog(parent, id, title, pos, size, style), 
imghelper{ imghelper },
outxt{ outxt }
{
    this->SetTitle("Algorithms");
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);

    button1 = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxSize(70, -1), 0);
    bSizer2->Add(button1, 0, wxALL, 5);


    bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    button2 = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize(70, -1), 0);
    bSizer3->Add(button2, 0, wxALL, 5);


    bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxVERTICAL);

    comboBox1 = new wxComboBox(this, wxID_ANY, wxT("Convert to Gray Scale"), wxDefaultPosition, wxSize(150, -1), 0, NULL, 0);
    bSizer4->Add(comboBox1, 0, wxALL, 5);


    bSizer1->Add(bSizer4, 1, wxEXPAND, 5);


    this->SetSizer(bSizer1);
    this->Layout();

    this->Centre(wxBOTH);

    fillComboInfo();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // set values
            int item = comboBox1->GetSelection();
            SelectionText = comboBox1->GetValue();
            tp t = start_tracking();
            DoFunction();
            wxString msg = end_tracking(t).c_str();
            outxt->writeTo(msg);
        });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // cancel
            Close();
        });
}

CInputDialog::~CInputDialog()
{
}

std::string
CInputDialog::setPath(bool Gray)
{
    auto spath = imghelper->getOriginalImage();
    imghelper->setFinalGray(Gray);
    return spath;
}

void
CInputDialog::setFinalImg(Mat& out)
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
CInputDialog::ApplyAlgorithm(Function1Parameter& f,
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
CInputDialog::ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size)
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
CInputDialog::ApplyAlgorithm(Function4Parameters& f,
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
CInputDialog::ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2)
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
CInputDialog::ApplyAlgorithm(Function5Parameters& f,
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

void
CInputDialog::ApplyAlgorithm(Function2Slider& f,
    bool Gray,
    double t)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            out = f(img, t);
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
        out = f(imghelper->getOrginalImageOpenCV(), t);
        setFinalImg(out);
    }

}

void
CInputDialog::ApplyAlgorithm(
    FunctionSobelParameters& f,
    bool Gray,
    int image_type,
    int depth,
    int type,
    double delta,
    int kernel_size)
{
    if (imghelper->getOriginalImageInitiated() == false)
    {
        Mat out;
        Mat img;
        if (loadImage(setPath(Gray), img) == true)
        {
            // using FunctionSobelParameters = std::function<Mat(Mat, int, int, int, double, int
            out = f(img, image_type, depth, type, delta, kernel_size);
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
        out = f(imghelper->getOrginalImageOpenCV(), image_type, depth, type, delta, kernel_size);
        setFinalImg(out);
    }

}


void CInputDialog::fillComboInfo()
{
    // Add functors to the functor mapping
    // this will allow the program to find the appropriate
    // algorithm to be applied
    fsimple["Convert to Gray Scale"] = convertograyScale;
    fsimple["Equalize Gray Scale Image"] = equalizeGrayImage;
    fsimple["Equalize Color Scale Image"] = equalizeColorImage;
    fsimple["Apply custom algo"] = ApplyCustomAlgo;
    fsimple["Invert Image"] = InvertImage;
    fsimple["Flip Image Horizontally"] = flipImageHorizontal;
    fsimple["Flip Image Vertically"] = flipImageVertical;
    fsimple["Flip Image"] = flipImage;
    fslider["Threshold"] = ApplyThreShold;
    fmore3["Canny Extended"] = ApplyCannyAlgoFull;
    fsimple["Sharpening"] = Sharpening;
    fsimple["Unsharp"] = Unsharp;
    fsimple["Hough Transform Custom"] = ApplyHoughTransformCustom;
    fsimple["Convert to Binary"] = getBinaryImage;
    fsimple["Erode"] = ApplyErode;
    fsimple["Dilate"] = ApplyDilate;
    fsimple["Closing"] = ApplyClosing;
    fsimple["Opening"] = ApplyOpening;
    fsimple["Morpholgical Gradient"] = ApplyMorphGradient;
    fsimple["Morphological Top Hat"] = ApplyTopHatAlgo;
    fmore["Blur Image"] = blurImageSmooth;
    fmore["Median"] = MedianImageSmooth;
    fmorep["Gaussian Extended"] = GaussianImageSmoothExtended;
    fmorepp["Laplacian Extended"] = ApplyLaplacianExtended;
    fadjust["Adjust Contrast"] = adjustContrast;
    fadjust["Adjust Brightness"] = adjustBrightness;
    fsobel["Sobel"] = ApplySobelExtended;
    fsimple["Neural"] = NN;

    // Now fill the combox box options with the algorithms

    // Basic operations
    comboBox1->Append("Convert to Gray Scale");
    comboBox1->Append("Equalize Gray Scale Image");
    comboBox1->Append("Equalize Color Scale Image");
    comboBox1->Append("Flip Image Horizontally");
    comboBox1->Append("Flip Image Vertically");
    comboBox1->Append("Flip Image");
    comboBox1->Append("Blur Image");
    comboBox1->Append("Invert Image");
    comboBox1->Append("Convert to Binary");
    comboBox1->Append("Threshold");
    comboBox1->Append("Adjust Contrast");
    comboBox1->Append("Adjust Brightness");

    // Morphological operations
    comboBox1->Append("Erode");
    comboBox1->Append("Dilate");
    comboBox1->Append("Closing");
    comboBox1->Append("Opening");
    comboBox1->Append("Morpholgical Gradient");
    comboBox1->Append("Morphological Top Hat");

    // Select ROI
    comboBox1->Append("Apply custom algo");

    // Gaussian Low pass filters
    comboBox1->Append("Gaussian Extended");

    // Median filters blurred images
    comboBox1->Append("Median");

    // Sharp algorithms
    comboBox1->Append("Sharpening");
    comboBox1->Append("Unsharp");

    // Lapalcian second derivative edge detectors
    comboBox1->Append("Laplacian Extended");

    comboBox1->Append("Sobel");

    // Gradient based algorithms
    comboBox1->Append("Canny Extended");

    // Circle detection algorithm
    comboBox1->Append("Hough Transform Custom");

    // feature detection algorithms
    comboBox1->Append("Harris Algorithm");

    comboBox1->Append("Neural");
}


std::function<Mat(Mat)>
CInputDialog::getAlgoFunctionOnePar(wxString key)
{
    if (fsimple.find(key) != fsimple.end())
    {
        return fsimple[key];
    }
    return nullptr;
}

std::function<Mat(Mat, int)>
CInputDialog::getAlgoFunctionTwoPar(wxString key)
{
    if (fmore.find(key) != fmore.end())
    {
        return fmore[key];
    }
    return nullptr;
}

std::function<Mat(Mat, int, double, double)>
CInputDialog::getAlgoFunctionFourPar(wxString key)
{
    if (fmorep.find(key) != fmorep.end())
    {
        return fmorep[key];
    }
    return nullptr;

}

std::function<Mat(Mat, int, int, int, int)>
CInputDialog::getAlgoFunctionFivePar(wxString key)
{
    if (fmorepp.find(key) != fmorepp.end())
    {
        return fmorepp[key];
    }
    return nullptr;

}

std::function<Mat(Mat, int, int)>
CInputDialog::getAlgoFunctionThreePar(wxString key)
{
    if (fmore3.find(key) != fmore3.end())
    {
        return fmore3[key];
    }
    return nullptr;
}

std::function<Mat(Mat, int)>
CInputDialog::getAlgoFunctionAdjust(wxString key)
{
    if (fadjust.find(key) != fadjust.end())
    {
        return fadjust[key];
    }
    return nullptr;
}

std::function<Mat(Mat, double)>
CInputDialog::getAlgoFunctionSlider(wxString key)
{
    if (fslider.find(key) != fslider.end())
    {
        return fslider[key];
    }
    return nullptr;
}

FunctionSobelParameters
CInputDialog::getAlgoSobel(wxString key)
{
    if (fsobel.find(key) != fsobel.end())
    {
        return fsobel[key];
    }
    return nullptr;
}

void CInputDialog::DoFunction()
{
    wxString opt = getSelectionText();

    outxt->writeTo("Applying algorithm: " + opt + " please wait...\n");

    Function1Parameter  f1 = getAlgoFunctionOnePar(opt);
    Function2Parameter  f2 = getAlgoFunctionTwoPar(opt);
    Function3Parameters f3 = getAlgoFunctionThreePar(opt);
    Function4Parameters f4 = getAlgoFunctionFourPar(opt);
    Function5Parameters f5 = getAlgoFunctionFivePar(opt);
    Function2Parameter  f6 = getAlgoFunctionAdjust(opt);
    Function2Slider     f7 = getAlgoFunctionSlider(opt);
    FunctionSobelParameters     f8 = getAlgoSobel(opt);

    if (f1 != nullptr)
    {
        ApplyAlgorithm(f1, true);
        return;
    }

    if (f2 != nullptr)
    {
        ApplyAlgorithm(f2, true, 3);
        return;
    }

    if (f3 != nullptr)
    {
        wxNumberEntryDialog dialog(this, "low threshold", "low threshold", "low threshold", 125, 1, 1000);
        int threshold = 0;
        int Aperture = 0;

        if (dialog.ShowModal() == wxID_OK)
        {
            threshold = dialog.GetValue();

            wxNumberEntryDialog dialog2(this, "high threshold", "high threshold", "high threshold", 350, 1, 1000);
            if (dialog2.ShowModal() == wxID_OK)
            {
                Aperture = dialog2.GetValue();
                ApplyAlgorithm(f3, true, threshold, Aperture);
            }
        }
        return;
    }

    if (f4 != nullptr)
    {
        wxNumberEntryDialog dialog(this, "Sigma factor", "Sigma Factor", "Divided by 100", 2, 1, 1000);
        if (dialog.ShowModal() == wxID_OK)
        {
            double sigma = 0.01 * static_cast<double>(dialog.GetValue());
            ApplyAlgorithm(f4, true, 3, sigma, sigma);
        }
        return;
    }

    if (f5 != nullptr)
    {
        wxNumberEntryDialog dialog(this, "Scale", "Scale", "Scale", 1, 1, 1000);
        int scale = 0;
        int delta = 0;

        if (dialog.ShowModal() == wxID_OK)
        {
            scale = dialog.GetValue();

            wxNumberEntryDialog dialog2(this, "Delta", "Delta", "Delta", 0, 0, 1000);
            if (dialog2.ShowModal() == wxID_OK)
            {
                delta = dialog2.GetValue();
                ApplyAlgorithm(f5, true, 3, scale, delta, CV_16S);
            }
        }
        return;
    }

    if (f6 != nullptr)
    {
        wxString tip;
        int max = 0;
        int min = 0;

        info inf;

        if (opt == "Adjust Contrast")
        {
            inf.default_value = 50;
            inf.max = 100;
            inf.min = 1;
            inf.title = opt;
            inf.default_value_string = "50";
        }
        else
        {
            inf.default_value = 50;
            inf.max = 255;
            inf.min = -255;
            inf.title = opt;
            inf.default_value_string = "50";
        }

        CSliderDialog dialog(this, inf);
        int scale = -90566;

        if (dialog.ShowModal() == wxID_CANCEL)
        {
            scale = dialog.getValue();

            if (scale == -90566)
            {
                return;
            }
            ApplyAlgorithm(f6, true, scale);
        }
        return;
    }

    if (f7 != nullptr)
    {
        if (opt == "Threshold")
        {
            double threshold = -90566.0;
            info inf;

            inf.default_value = 50;
            inf.max = 255;
            inf.min = 0;
            inf.title = "Threshold";
            inf.default_value_string = "50";

            CSliderDialog dialog(this, inf);
            auto nice = dialog.ShowModal();
            if (nice == wxID_CANCEL)
            {
                auto v = dialog.getValue();

                if (v == -90566.0)
                {
                    return;
                }

                threshold = static_cast<double>(v);
                ApplyAlgorithm(f7, true, threshold);
            }
            return;
        }
    }

    if (f8 != nullptr)
    {

        int depth = 10;
        int image_type = CV_8U;
        int type = 0;
        double delta = 10.0;
        int kernel_size = 5;

        wxString tip = "Depth";
        int max = 1000;
        int min = 1;

        wxNumberEntryDialog dialog1(this, opt, tip, opt, 10, min, max);

        if (dialog1.ShowModal() == wxID_OK)
        {
            depth = dialog1.GetValue();
        }

        tip = "Type";
        max = 10;
        min = 1;

        wxNumberEntryDialog dialog2(this, opt, tip, opt, 10, min, max);
        if (dialog2.ShowModal() == wxID_OK)
        {
            type = dialog2.GetValue();
        }

        tip = "Delta";
        max = 1000;
        min = 1;

        wxNumberEntryDialog dialog3(this, opt, tip, opt, 10, min, max);
        if (dialog3.ShowModal() == wxID_OK)
        {
            delta = dialog3.GetValue();
        }

        tip = "Kernel Size";
        max = 13;
        min = 3;

        wxNumberEntryDialog dialog4(this, opt, tip, opt, 3, min, max);
        if (dialog4.ShowModal() == wxID_OK)
        {
            type = dialog4.GetValue();
        }

        ApplyAlgorithm(f8, true, image_type, depth, type, delta, kernel_size);
    }
}


