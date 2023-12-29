#include "childframes.h"
#include "constants.h"
#include <chrono>
#include <ctime>
#include <wx/textdlg.h>
#include "image_interest_points.h"
#include "filesys.h"


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

CInputDialog::CInputDialog(     wxWindow* parent,
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
            Mat out = DoFunction();            
            Size s = out.size();
            if (s.width != 0 && s.height != 0)
            {
                setFinalImg(out);
            }
 
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


template<typename F, typename...Args>
Mat
CInputDialog::ApplyAlgorithmEffective(F& f, bool Gray, Args&&... args)
{
    try
    {
        if (imghelper->getOriginalImageInitiated() == false)
        {
            Mat out;
            Mat img;
            if (loadImage(setPath(Gray), img) == true)
            {
                wxBusyInfo* wait = ProgramBusy();
                out = f(img, args ...);
                Stop(wait);
                auto s = out.size();
                if (s.height == 0 || s.width == 0)
                {
                    return out;
                }
                //setFinalImg(out);
            }
        }
        else
        {
            Mat out;
            wxBusyInfo* wait = ProgramBusy();
            out = f(imghelper->getOrginalImageOpenCV(), args ...);
            Stop(wait);
            return out;
            //setFinalImg(out);
        }
    }
    catch (...)
    {
        Mat empty;
        return empty;
    }


    Mat empty;
    return empty;
}

Mat
CInputDialog::ApplyAlgorithm(Function1Parameter& f,bool Gray)
{
    return ApplyAlgorithmEffective(f, Gray);
}

Mat
CInputDialog::ApplyAlgorithm(Function2Parameter& f,bool Gray, int kernel_size)
{
    return ApplyAlgorithmEffective(f, Gray, kernel_size);
}

Mat
CInputDialog::ApplyAlgorithm(Function4Parameters& f, bool Gray, int kernel_size,double p1, double p2)
{
    return ApplyAlgorithmEffective(f, Gray, kernel_size, p1, p2);
}

Mat
CInputDialog::ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2)
{
    return ApplyAlgorithmEffective(f, Gray, p1, p2);
}

Mat
CInputDialog::ApplyAlgorithm(Function5Parameters& f, bool Gray,int kernel_size,int p1,int p2,int p3)
{
    return ApplyAlgorithmEffective(f, Gray, kernel_size, p1, p2, p3);
}

Mat
CInputDialog::ApplyAlgorithm(Function2Slider& f, bool Gray, double t)
{
    return ApplyAlgorithmEffective(f, Gray, t);
}

Mat
CInputDialog::ApplyAlgorithm(
                                FunctionSobelParameters& f,
                                bool Gray,
                                int image_type,
                                int depth,
                                int type,
                                double delta,
                                int kernel_size
                            )
{
    return ApplyAlgorithmEffective(f, Gray, image_type, depth, type, delta, kernel_size);
}

Mat CInputDialog::ApplyAlgorithm(
                                FunctionHarris& f, 
                                bool Gray, 
                                int neighborhood_size, 
                                int aperture_size, 
                                double threshold, 
                                double Harris_parameter)
{
    return ApplyAlgorithmEffective(f, Gray, neighborhood_size, aperture_size, threshold, Harris_parameter);
}

void CInputDialog::setSimpleMaps()
{
    fsimple["Convert to Gray Scale"] = convertograyScale;
    fsimple["Equalize Gray Scale Image"] = equalizeGrayImage;
    fsimple["Equalize Color Scale Image"] = equalizeColorImage;
    fsimple["Apply custom algo"] = ApplyCustomAlgo;
    fsimple["Invert Image"] = InvertImage;
    fsimple["Flip Image Horizontally"] = flipImageHorizontal;
    fsimple["Flip Image Vertically"] = flipImageVertical;
    fsimple["Flip Image"] = flipImage;
    fsimple["Difference of Gaussians"] = ApplyDIfferenceOfGaussian;
    fsimple["Sharpening"] = Sharpening;
    fsimple["Unsharp"] = Unsharp;
    fsimple["Hough Transform Lines"] = ApplyHoughTransformLines;
    fsimple["Hough Transform Circles"] = ApplyHoughTransformCircles;
    fsimple["Convert to Binary"] = getBinaryImage;
    fsimple["Erode"] = ApplyErode;
    fsimple["Dilate"] = ApplyDilate;
    fsimple["Closing"] = ApplyClosing;
    fsimple["Opening"] = ApplyOpening;
    fsimple["Morpholgical Gradient"] = ApplyMorphGradient;
    fsimple["Morphological Top Hat"] = ApplyTopHatAlgo;
    fsimple["Segmentation Erode"] = segmentErode;
    fsimple["Find Contourns ( Threshold )"] = ApplyFindContournsThreshold;
    fsimple["Find Contourns ( Canny )"] = ApplyFindContournsCanny;

}

void CInputDialog::setOtherMaps()
{
    fmore["Erosion+"] = ApplyErodeEx;
    fmore["Dilate+"] = ApplyDilateEx;
    fmore["Blur Image"] = blurImageSmooth;
    fmore["Median"] = MedianImageSmooth;
    fmore3["Canny Extended"] = ApplyCannyAlgoFull;
    fmorep["Gaussian Extended"] = GaussianImageSmoothExtended;
    fmorepp["Laplacian Extended"] = ApplyLaplacianExtended;
    fadjust["Adjust Contrast"] = adjustContrast;
    fadjust["Adjust Brightness"] = adjustBrightness;
    fsobel["Sobel"] = ApplySobelExtended;
    fslider["Threshold"] = ApplyThreShold;
    fslider["Gamma Correction"] = adjustGama;
    fharris["Harris Algorithm"] = detectCornersHarrisAlgoFull;
}

void CInputDialog::fillComboInfo()
{
    // Add functors to the functor mapping
    // this will allow the program to find the appropriate
    // algorithm to be applied
    setSimpleMaps();
    setOtherMaps();
    // Now fill the combox box options with the algorithms

    for (const auto& name : image_constants::_algorithms_)
    {
        comboBox1->Append(name);
    }
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

FunctionHarris
CInputDialog::getAlgoHarris(wxString key)
{
    if (fharris.find(key) != fharris.end())
    {
        return fharris[key];
    }
    return nullptr;
}

double Distance(const ImageDescriptors& lhs, const ImageDescriptors& rhs)
{
    int xlhs = lhs.centroid.first;
    int ylhs = lhs.centroid.second;
    int xrhs = rhs.centroid.first;
    int yrhs = rhs.centroid.second;

    return sqrt(pow(xrhs - xlhs, 2) + pow(yrhs - ylhs, 2));
}

Mat CInputDialog::DoFunction()
{
    wxString _algorithm = getSelectionText();

    outxt->writeTo("Applying algorithm: " + _algorithm + " please wait...\n");

    if (_algorithm == "Undo")
    {
        if (imghelper->getOriginalImageInitiated() == true)
        {
            if (imghelper->revert() == false)
            {
                outxt->writeTo("Error, final image not loaded\n");
            }
        }
        shouldQuit = true;
        Mat empty;
        return empty;
    }

    if (_algorithm == "Find Contourns Descriptors")
    {
        if (imghelper->getOriginalImageInitiated() == true)
        {
            Mat img = imghelper->getOrginalImageOpenCV();
            std::string s = imghelper->getOriginalImage();
            std::string s2 = getFileName(s);

            Descriptors descriptors = image_info::getImageDescriptors(img);
            std::stringstream os;

            if (directory_exists("out") == false)
            {
                create_dir("out");
            };
            os <<"out\\" << getFileName(s2) << ".csv";

            wxBusyInfo* wait = ProgramBusy();
            image_info::createCSV(descriptors, os.str());
            Stop(wait);
            std::string msg = "csv file " + os.str() + " has been created";
            outxt->writeTo(msg.c_str());
        }
        shouldQuit = true;
        Mat empty;
        return empty;
    }

    Function1Parameter  function1P  = getAlgoFunctionOnePar(_algorithm);

    if (function1P != nullptr)
    {
        Mat out = ApplyAlgorithm(function1P, true);
        shouldQuit = true;
        return out;
    }

    Function2Parameter  function2P = getAlgoFunctionTwoPar(_algorithm);

    if (function2P != nullptr)
    {
        int option = 3;
        if (_algorithm == "Erosion+" || _algorithm == "Dilate+")
        {      
            std::vector<wxString> choices = { "MORPH_RECT","MORPH_CROSS","MORPH_ELLIPSE" };
            wxSingleChoiceDialog dialog(
                                            this, 
                                            "Choose Basic Element", 
                                            "Choose Basic Element",
                                            static_cast<int>(choices.size()), choices.data());

            dialog.ShowModal();
            wxString structural_element = dialog.GetStringSelection();
            if (structural_element == "MORPH_RECT")
            {
                option = MORPH_RECT;
            }
            else
            if (structural_element == "MORPH_CROSS")
            {
                option = MORPH_CROSS;
            }
            else
            if (structural_element == "MORPH_ELLIPSE")
            {
                option = MORPH_ELLIPSE;
            }
            else
            {
                shouldQuit = true;
                Mat empty;
                return empty;
            }
 
        }
        Mat out = ApplyAlgorithm(function2P, true, option);
        shouldQuit = true;
        return out;
    }

    Function3Parameters function3P = getAlgoFunctionThreePar(_algorithm);

    if (function3P != nullptr)
    {
        wxNumberEntryDialog dialog2(this, "low threshold", "low threshold", "low threshold", 125, 1, 1000);
        int threshold = 0;
        int Aperture = 0;

        Mat out;

        if (dialog2.ShowModal() == wxID_OK)
        {
            threshold = dialog2.GetValue();

            wxNumberEntryDialog dialog3(this, "high threshold", "high threshold", "high threshold", 350, 1, 1000);
            if (dialog3.ShowModal() == wxID_OK)
            {
                Aperture = dialog3.GetValue();
                out = ApplyAlgorithm(function3P, true, threshold, Aperture);
            }
        }
        shouldQuit = true;
        return out;
    }

    Function4Parameters function4P = getAlgoFunctionFourPar(_algorithm);

    if (function4P != nullptr)
    {

        double sigma = INVALID_VALUE_DOUBLE;
        CNumentryDouble dialog1(this, 2000, 0, 1, wxID_ANY, "Sigma Value");
        dialog1.ShowModal();
        sigma = dialog1.getComponentValue()->GetValue();
        Mat out;

        if (sigma != INVALID_VALUE_DOUBLE)
        {
            CNumentryDouble dialog2(this, 1000, 0, 100, wxID_ANY, "Division factor");
            dialog2.ShowModal();
            double factor = dialog2.getComponentValue()->GetValue();
            if (factor == 0.0)
            {
                factor = 1.0;
            }
            out = ApplyAlgorithm(function4P, true, 3, sigma/factor, sigma/factor);
        }
        shouldQuit = true;
        return out;
    }

    Function5Parameters function5P = getAlgoFunctionFivePar(_algorithm);
    if (function5P != nullptr)
    {
        wxNumberEntryDialog dialog1(this, "Scale", "Scale", "Scale", 1, 1, 1000);
        int scale = 0;
        int delta = 0;
        Mat out;

        if (dialog1.ShowModal() == wxID_OK)
        {
            scale = dialog1.GetValue();

            wxNumberEntryDialog dialog2(this, "Delta", "Delta", "Delta", 0, 0, 1000);
            if (dialog2.ShowModal() == wxID_OK)
            {
                delta = dialog2.GetValue();
                out = ApplyAlgorithm(function5P, true, 3, scale, delta, CV_16S);
            }
        }
        shouldQuit = true;
        return out;
    }

    Function2Parameter  functionAdjust = getAlgoFunctionAdjust(_algorithm);

    if (functionAdjust != nullptr)
    {
        wxString tip;
        int max = 0;
        int min = 0;

        info inf;
        Mat out;

        if (_algorithm == "Adjust Contrast")
        {
            inf.default_value = 50;
            inf.max = 100;
            inf.min = 1;
            inf.title = _algorithm;
            inf.default_value_string = "50";
        }
        else
        {
            inf.default_value = 50;
            inf.max = 255;
            inf.min = -255;
            inf.title = _algorithm;
            inf.default_value_string = "50";
        }

        CSliderDialog dialog1(this, inf);
        int scale = INVALID_VALUE_INT;

        dialog1.ShowModal();

        scale = dialog1.getValue();

        if (scale == INVALID_VALUE_INT)
        {
            shouldQuit = true;
            return out;
        }
        out = ApplyAlgorithm(functionAdjust, true, scale);
        shouldQuit = true;
        return out;
    }

    Function2Slider     functionSlider = getAlgoFunctionSlider(_algorithm);

    if (functionSlider != nullptr)
    {
        Mat out;
        if (_algorithm == "Threshold" || _algorithm == "Gamma Correction")
        {
            double threshold = 0.0;
            info inf;            

            if (_algorithm == "Threshold")
            {
                inf.default_value = 50;
                inf.max = 255;
                inf.min = 0;
                inf.title = "Threshold";
                inf.default_value_string = "50";
            }
            else
            {
                inf.default_value = 50;
                inf.max = 1000;
                inf.min = 1;
                inf.title = "Gamma Correction %";
                inf.default_value_string = "50";
            }

            double v = INVALID_VALUE_DOUBLE;

            CSliderDialog dialog(this, inf);
            dialog.ShowModal();

            v = dialog.getValue();

            if (v == INVALID_VALUE_DOUBLE)
            {
                shouldQuit = true;
                return out;
            }

            if (_algorithm == "Threshold")
            {
                threshold = static_cast<double>(v);
            }
            else
            {
                threshold = static_cast<double>(v)/100;
            }
                
            out = ApplyAlgorithm(functionSlider, true, threshold);

        }
        shouldQuit = true;
        return out;
    }

    FunctionSobelParameters     functionS = getAlgoSobel(_algorithm);

    if (functionS != nullptr)
    {
        Mat out;
        int depth = 10;
        int image_type = CV_8U;
        int type = 0;
        int max = 1000;
        int min = 1;
        double delta = 10.0;
        int kernel_size = 5;
        wxString tip = "Depth";

        wxNumberEntryDialog dialog1(this, _algorithm, tip, _algorithm, 10, min, max);

        if (dialog1.ShowModal() == wxID_OK)
        {
            depth = dialog1.GetValue();
        }

        tip = "Type";
        max = 10;
        min = 1;

        wxNumberEntryDialog dialog2(this, _algorithm, tip, _algorithm, 10, min, max);
        if (dialog2.ShowModal() == wxID_OK)
        {
            type = dialog2.GetValue();
        }

        tip = "Delta";
        max = 1000;
        min = 1;

        wxNumberEntryDialog dialog3(this, _algorithm, tip, _algorithm, 10, min, max);
        if (dialog3.ShowModal() == wxID_OK)
        {
            delta = dialog3.GetValue();
        }

        tip = "Kernel Size";
        max = 13;
        min = 3;

        wxNumberEntryDialog dialog4(this, _algorithm, tip, _algorithm, 3, min, max);
        if (dialog4.ShowModal() == wxID_OK)
        {
            type = dialog4.GetValue();
        }
        out = ApplyAlgorithm(functionS, true, image_type, depth, type, delta, kernel_size);
        shouldQuit = true;
        return out;
    }

    FunctionHarris farris = getAlgoHarris(_algorithm);

    if (farris != nullptr)
    {
        Mat out;
        out = ApplyAlgorithm(farris, true, 3, 2, 0.001, 0.0001);
        shouldQuit = true;
        return out;
    }

    Mat empty;
    return empty;
}


