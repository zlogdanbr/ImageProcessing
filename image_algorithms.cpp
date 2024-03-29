#include "childframes.h"
#include "constants.h"
#include <chrono>
#include <ctime>
#include <wx/textdlg.h>
#include "wx/msgdlg.h"
#include "image_interest_points.h"
#include "filesys.h"
#include "pca.h"
#include <fstream>
#include <CvPlot/cvplot.h>
#include <matplot/matplot.h>


CInputDialog::CInputDialog(     wxWindow* parent,
                                const Mat& original,
                                wxWindowID id, 
                                const wxString& title, 
                                const wxPoint& pos, 
                                const wxSize& size, 
                                long style): 
                                wxDialog(parent, id, title, pos, size, style),
                                original{original}
{
    actual_first = original.clone();
    this->SetTitle("Algorithms");
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxHORIZONTAL);

    button1 = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxSize(70, -1), 0);
    bSizer2->Add(button1, 0, wxALL, 5);

    button2 = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer2->Add(button2, 0, wxALL, 5);


    bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer3;
    bSizer3 = new wxBoxSizer(wxVERTICAL);

    button3 = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize(70, -1), 0);
    bSizer3->Add(button3, 0, wxALL, 5);


    bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

    wxBoxSizer* bSizer4;
    bSizer4 = new wxBoxSizer(wxVERTICAL);

    comboBox1 = new wxComboBox(this, wxID_ANY, wxT("Convert to Gray Scale"), wxDefaultPosition, wxSize(200, -1), 0, NULL, 0);
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
            destroyAllWindows();
            DoFunction();            
            Size s = final_image.size();
            if (s.width != 0 && s.height != 0)
            {
                showImage(final_image, "Temporary");
            } 
        });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // cancel
            destroyAllWindows();
            Close();
        });

    // op_cancelled
    button3->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // cancel
            op_cancelled = true;
            destroyAllWindows();
            Close();
        });


}

CInputDialog::~CInputDialog()
{
}

template<typename F, typename...Args>
void
CInputDialog::ApplyAlgorithmEffective(F& f, bool Gray, Args&&... args)
{
    if (original.empty() == false)
    {
        wxBusyInfo* wait = ProgramBusy();
        final_image = f(original, args ...);
        Stop(wait);
        setOriginalImage();
    }
}

void
CInputDialog::ApplyAlgorithm(Function1Parameter& f,bool Gray)
{
    ApplyAlgorithmEffective(f, Gray);
}

void
CInputDialog::ApplyAlgorithm(Function2Parameter& f,bool Gray, int kernel_size)
{
    ApplyAlgorithmEffective(f, Gray, kernel_size);
}

void
CInputDialog::ApplyAlgorithm(Function4Parameters& f, bool Gray, int kernel_size,double p1, double p2)
{
   ApplyAlgorithmEffective(f, Gray, kernel_size, p1, p2);
}

void
CInputDialog::ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2)
{
    ApplyAlgorithmEffective(f, Gray, p1, p2);
}

void
CInputDialog::ApplyAlgorithm(Function5Parameters& f, bool Gray,int kernel_size,int p1,int p2,int p3)
{
    ApplyAlgorithmEffective(f, Gray, kernel_size, p1, p2, p3);
}

void
CInputDialog::ApplyAlgorithm(Function2Slider& f, bool Gray, double t)
{
    ApplyAlgorithmEffective(f, Gray, t);
}

void
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
    fsimple["Gaussian Difference"] = ApplyDifferenceOfGaussian;
    fsimple["Show Sift Descriptors"] = ApplySiftToImage;
    fsimple["Find Faces"] = FindFacesAndDrawRectangles;

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

double Distance(const ImageDescriptors& lhs, const ImageDescriptors& rhs)
{
    int xlhs = lhs.centroid.first;
    int ylhs = lhs.centroid.second;
    int xrhs = rhs.centroid.first;
    int yrhs = rhs.centroid.second;

    return sqrt(pow(xrhs - xlhs, 2) + pow(yrhs - ylhs, 2));
}

void ShowPCA(eigenSpace& _espace)
{
    eigenvectors evectors = _espace.first;
    eigenvalues  evalues = _espace.second;

    std::vector<double> x1;    
    std::vector<double> y1;

    for (const auto& ev : evectors)
    {
        // using eigenvector = std::vector<Point2d>;
        // using eigenvectors = std::vector< eigenvector >;
        // using eigenvalues = std::vector< eigenvalue  >;
        // using eigenvalue = std::vector<double>;

        for (const auto& e : ev)
        {
            x1.push_back(e.x);
            y1.push_back(e.y);
        }

    }
    
    matplot::feather(x1, y1);
    matplot::show();

}


bool CInputDialog::DoFunctionBasedOnNameAlgo(wxString& _algorithm)
{

    if (_algorithm == "Create PCA file")
    {
        if (original.empty() == false)
        {
            eigenSpace _espace;
            wxBusyInfo* wait = ProgramBusy();
            std::stringstream os = getEingenSpaceInfo(original, _espace);
            Stop(wait);

            if (wxYES == wxMessageBox(  wxT("Save file?"),
                                        wxT("Save file?"),
                                        wxNO_DEFAULT | wxYES_NO | wxCANCEL | wxICON_INFORMATION,
                                        this))
            {

                wxFileDialog saveFileDialog(this,
                                            wxEmptyString,
                                            wxEmptyString,
                                            "pca.csv",
                                            "Text Files (*.csv)|*.csv|All Files (*.*)|*.*",
                                            wxFD_SAVE);

                if (saveFileDialog.ShowModal() == wxID_OK)
                {
                    wxString spath = saveFileDialog.GetPath();
                    std::string path = convertWxStringToString(spath);

                    std::ofstream myfile(path);
                    if (myfile.is_open())
                    {
                        myfile << os.str();
                    }
                    myfile.close();
                }
            }

            // let's show some graphics
            ShowPCA(_espace);
        } 
        return true;
    }

    if (_algorithm == "Undo")
    {
        if (revertContainer.isEmpty() == false)
        {
            revert();
        }
        return true;
    }

    if (_algorithm == "Apply Custom Kernel")
    {
        if (original.empty() == false)
        {
            CGridDialogInput* dialog = new CGridDialogInput(this, original, wxID_ANY, "Custom Kernel Selection");
            dialog->ShowModal();
            final_image = dialog->getFinal();
            setOriginalImage();
            delete dialog;
        }
        return true;
    }

    if (_algorithm == "Histogram")
    {
        if (original.empty() == false)
        {
            plotHistogram(original);
        }
        return true;
    }

    if (_algorithm == "Find Sift Descriptors")
    {
        if (original.empty() == false)
        {
            Mat clone = original.clone();
            Mat descriptors;
            wxBusyInfo* wait = ProgramBusy();
            std::vector < cv::KeyPoint >  kp = sift_algo::ApplySift(clone, descriptors);
            Stop(wait);

            if (wxYES == wxMessageBox(wxT("Save file?"),
                wxT("Save file?"),
                wxNO_DEFAULT | wxYES_NO | wxCANCEL | wxICON_INFORMATION,
                this))
            {

                wxFileDialog saveFileDialog(this,
                    wxEmptyString,
                    wxEmptyString,
                    "sift.csv",
                    "Text Files (*.csv)|*.csv|All Files (*.*)|*.*",
                    wxFD_SAVE);

                if (saveFileDialog.ShowModal() == wxID_OK)
                {
                    wxString spath = saveFileDialog.GetPath();
                    std::string path = convertWxStringToString(spath);
                    sift_algo::createCSV(kp, path);
                }
            }
        }
        setOriginalImage();
        return true;
    }

    if (_algorithm == "Crop Image")
    {
        if (original.empty() == false)
        {

            int M = 0;
            int N = 0;
            Mat out;

            wxNumberEntryDialog* dialogCrop1 = new wxNumberEntryDialog(this, "Width of window", "Crop size", "Crop size", 8, 4, 128);
            if (dialogCrop1->ShowModal() == wxID_OK)
            {
                M = dialogCrop1->GetValue();
            }
            else
            {
                return true;
            }

            if (dialogCrop1 != nullptr)
            {
                delete dialogCrop1;
            }
            dialogCrop1 = nullptr;

            dialogCrop1 = new wxNumberEntryDialog(this, "Height of window", "Crop size", "Crop size", 8, 4, 128);
            if (dialogCrop1->ShowModal() == wxID_OK)
            {
                N = dialogCrop1->GetValue();
            }
            else
            {
                if (dialogCrop1 != nullptr)
                {
                    delete dialogCrop1;
                }
                return true;
            }
            std::vector<Mat> v;
            final_image = cropImage(original, M, N, v);
            setOriginalImage();
            if (dialogCrop1 != nullptr)
            {
                delete dialogCrop1;
            }
        }
        return true;
    }

    return false;
}

bool CInputDialog::DoFunctionBasedOnFunctor(wxString& _algorithm)
{
    Function1Parameter  function1P = getAlgoFunctionOnePar(_algorithm);

    if (function1P != nullptr)
    {
        ApplyAlgorithm(function1P, true);
        return true;
    }

    Function2Parameter  function2P = getAlgoFunctionTwoPar(_algorithm);

    if (function2P != nullptr)
    {
        int option = MORPH_CROSS;
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
                return true;
            }

        }
        ApplyAlgorithm(function2P, true, option);
        shouldQuit = true;
        return true;
    }

    Function3Parameters function3P = getAlgoFunctionThreePar(_algorithm);

    if (function3P != nullptr)
    {
        wxNumberEntryDialog* dialog2 = new wxNumberEntryDialog(this, "low threshold", "low threshold", "low threshold", 125, 1, 1000);
        int threshold = 0;
        int Aperture = 0;

        Mat out;

        if (dialog2->ShowModal() == wxID_OK)
        {
            threshold = dialog2->GetValue();

            dialog2 = nullptr;

            dialog2 = new wxNumberEntryDialog(this, "high threshold", "high threshold", "high threshold", 350, 1, 1000);
            if (dialog2->ShowModal() == wxID_OK)
            {
                Aperture = dialog2->GetValue();
                ApplyAlgorithm(function3P, true, threshold, Aperture);
            }
        }

        if (dialog2 != nullptr)
        {
            delete dialog2;
        }

        shouldQuit = true;
        return true;
    }

    Function4Parameters function4P = getAlgoFunctionFourPar(_algorithm);

    if (function4P != nullptr)
    {

        double sigma = INVALID_VALUE_DOUBLE;
        CNumentryDouble* dialog1 = new CNumentryDouble(this, 10000, 0, 1, wxID_ANY, "Sigma Value");
        dialog1->ShowModal();
        sigma = dialog1->getComponentValue()->GetValue();
        Mat out;

        if (sigma != INVALID_VALUE_DOUBLE)
        {
            dialog1 = nullptr;
            dialog1 = new CNumentryDouble(this, 10000, 0, 100, wxID_ANY, "Division factor");
            dialog1->ShowModal();
            double factor = dialog1->getComponentValue()->GetValue();
            if (factor == 0.0)
            {
                factor = 1.0;
            }
            ApplyAlgorithm(function4P, true, 3, sigma / factor, sigma / factor);
        }
        shouldQuit = true;

        if (dialog1 != nullptr)
        {
            delete dialog1;
        }
        return true;
    }

    Function5Parameters function5P = getAlgoFunctionFivePar(_algorithm);
    if (function5P != nullptr)
    {
        wxNumberEntryDialog* dialog1 = new wxNumberEntryDialog(this, "Scale", "Scale", "Scale", 1, 1, 1000);
        int scale = 0;
        int delta = 0;
        Mat out;

        if (dialog1->ShowModal() == wxID_OK)
        {
            scale = dialog1->GetValue();

            dialog1 = nullptr;
            dialog1 = new wxNumberEntryDialog(this, "Delta", "Delta", "Delta", 0, 0, 1000);
            if (dialog1->ShowModal() == wxID_OK)
            {
                delta = dialog1->GetValue();
                ApplyAlgorithm(function5P, true, 3, scale, delta, CV_16S);
            }
        }

        if (dialog1 != nullptr)
        {
            delete dialog1;
        }
        shouldQuit = true;
        return true;
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
            return true;
        }
        ApplyAlgorithm(functionAdjust, true, scale);
        shouldQuit = true;
        return true;
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
                return true;
            }

            if (_algorithm == "Threshold")
            {
                threshold = static_cast<double>(v);
            }
            else
            {
                threshold = static_cast<double>(v) / 100;
            }

            ApplyAlgorithm(functionSlider, true, threshold);

        }
        shouldQuit = true;
        return true;
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

        wxNumberEntryDialog* dialog = new wxNumberEntryDialog(this, _algorithm, tip, _algorithm, 10, min, max);

        if (dialog->ShowModal() == wxID_OK)
        {
            depth = dialog->GetValue();
        }

        tip = "Type";
        max = 10;
        min = 1;

        dialog = nullptr;
        dialog = new wxNumberEntryDialog(this, _algorithm, tip, _algorithm, 10, min, max);
        if (dialog->ShowModal() == wxID_OK)
        {
            type = dialog->GetValue();
        }

        tip = "Delta";
        max = 1000;
        min = 1;

        if (dialog != nullptr)
        {
            delete dialog;
        }
        dialog = nullptr;

        dialog = new wxNumberEntryDialog(this, _algorithm, tip, _algorithm, 10, min, max);
        if (dialog->ShowModal() == wxID_OK)
        {
            delta = dialog->GetValue();
        }

        tip = "Kernel Size";
        max = 13;
        min = 3;

        dialog = new wxNumberEntryDialog(this, _algorithm, tip, _algorithm, 3, min, max);
        if (dialog->ShowModal() == wxID_OK)
        {
            type = dialog->GetValue();
        }
        ApplyAlgorithm(functionS, true, image_type, depth, type, delta, kernel_size);
        shouldQuit = true;

        if (dialog != nullptr)
        {
            delete dialog;
        }
        return true;
    }

    return false;
}

void CInputDialog::DoFunction()
{
    wxString _algorithm = getSelectionText();

    if (DoFunctionBasedOnNameAlgo(_algorithm) == true)
    {
        return;
    }

    if (DoFunctionBasedOnFunctor(_algorithm) == true)
    {
        return;
    }
 
}


