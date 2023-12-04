#include "childframes.h"


CInputDialog::CInputDialog(wxFrame* parent) :CInputDialogBase{ parent,"Basic Algorithms Selection" }
{
    setControlslayout();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // set values
            int item = comboBox1->GetSelection();
            SelectionText = comboBox1->GetValue();
            DoFunction();
            Close();
        });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // cancel
            Close();
        });
}

void CInputDialog::setControlslayout()
{
    this->SetSize(350, 70);
    // set base sizer
    basePanel->SetSize(350, 70);
    basePanel->SetSizer(baseSizer);

    // add buttons to the horizontal box
    hbox1->Add(button1);
    hbox1->Add(button2);

    // add buttons to the horizontal box
    hbox1->Add(comboBox1);

    // set horizontal base sizer at panel1
    panel1->SetSize(350, 70);
    panel1->SetSizer(hbox1);

    // add panel1 to the base sizer at the base panel
    baseSizer->Add(panel1);

    fillComboInfo();

    Center();
}


void CInputDialog::fillComboInfo()
{

    // Add functors to the functor mapping
    // this will allow the program to find the appropriate
    // algorithm to be applied
    fsimple["Convert to Gray Scale"] = convertograyScale;
    fsimple["Equalize Gray Scale Image"] = equalizeGrayImage;
    fsimple["Equalize Color Scale Image"] = equalizeColorImage;
    fsimple["Laplacian"] = ApplyLaplacian;
    fsimple["Harris Algorithm"] = detectCornersHarris;
    fsimple["Detect features"] = detectFastKeyPoints;
    fsimple["Working Algorithm"] = workingAlgorithm;
    fsimple["Detect Corners"] = detectCornersHarris;
    fsimple["Detect features"] = detectFastKeyPoints;
    fsimple["Invert Image"] = InvertImage;
    fsimple["Flip Image Horizontally"] = flipImageHorizontal;
    fsimple["Flip Image Vertically"] = flipImageVertical;
    fsimple["Flip Image"] = flipImage;
    fsimple["Threshold"] = ApplyThreShold;
    fsimple["Canny"] = ApplyCanny;
    fmore3["Canny Extended"] = ApplyCannyAlgoFull;
    fsimple["Sharpening"] = Sharpening;
    fsimple["Unsharp"] = Unsharp;
    fsimple["Hough Transform"] = ApplyHoughTransformRegular;
    fsimple["Hough Transform Probabilistic"] = ApplyHoughTransformReProbabilistic;
    fsimple["Hough Transform Custom"] = ApplyHoughTransformCustom;
    fsimple["Convert to Binary"] = getBinaryImage;
    fsimple["Watershed"] = WaterShed;
    fsimple["Erode"] = ApplyErode;
    fsimple["Dilate"] = ApplyDilate;
    fsimple["Morpholgical Gradient"] = ApplyMorphGradient;
    fsimple["Morphological Top Hat"] = ApplyTopHatAlgo;
    fmore["Blur Image"] = blurImageSmooth;
    fmore["Gaussian"] = GaussianImageSmooth;
    fmore["Median"] = MedianImageSmooth;
    fmore["Sobel"] = ApplySobel;
    fmorep["Gaussian Extended"] = GaussianImageSmoothExtended;
    fmorepp["Laplacian Extended"] = ApplyLaplacianExtended;

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

    // Morphological operations
    comboBox1->Append("Erode");
    comboBox1->Append("Dilate");
    comboBox1->Append("Morpholgical Gradient");
    comboBox1->Append("Morphological Top Hat");

    // Custom algorithms I am working
    comboBox1->Append("Working Algorithm");

    // Gaussian Low pass filters
    comboBox1->Append("Gaussian");
    comboBox1->Append("Gaussian Extended");

    // Median filters blurred images
    comboBox1->Append("Median");

    // Sharp algorithms
    comboBox1->Append("Sharpening");
    comboBox1->Append("Unsharp");
    
    // Lapalcian second derivative edge detectors
    comboBox1->Append("Laplacian");
    comboBox1->Append("Laplacian Extended");

    // Gradient based algorithms
    comboBox1->Append("Sobel");
    comboBox1->Append("Canny");
    comboBox1->Append("Canny Extended");

    // Circle detection algorithm
    comboBox1->Append("Hough Transform");
    comboBox1->Append("Hough Transform Probabilistic");
    comboBox1->Append("Hough Transform Custom");

    // feature detection algorithms
    comboBox1->Append("Harris Algorithm");
    comboBox1->Append("Detect features");

    // Watershed
    comboBox1->Append("Watershed");
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

void CInputDialog::DoFunction()
{
    outxt->writeTo("Applying algorithm... please wait...\n");
    wxString opt = getSelectionText();

    Function1Parameter  f1 = getAlgoFunctionOnePar(opt);
    Function2Parameter  f2 = getAlgoFunctionTwoPar(opt);
    Function3Parameters f3 = getAlgoFunctionThreePar(opt);
    Function4Parameters f4 = getAlgoFunctionFourPar(opt);
    Function5Parameters f5 = getAlgoFunctionFivePar(opt);

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
        wxNumberEntryDialog dialog(this, "threshold", "threshold", "threshold", 1, 1, 1000);
        int threshold = 0;
        int Aperture = 0;

        if (dialog.ShowModal() == wxID_OK)
        {
            threshold = dialog.GetValue();

            wxNumberEntryDialog dialog2(this, "Aperture", "Aperture", "Aperture", 1, 1, 1000);
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
        wxNumberEntryDialog dialog(this, "Sigma factor", "Sigma Factor", "Divided by 10", 2, 1, 1000);
        if (dialog.ShowModal() == wxID_OK)
        {
            double sigma = 0.1 * static_cast<double>(dialog.GetValue());
            ApplyAlgorithm(f4, true, 3, sigma, sigma);
        }
        return;
    }

    if (f5 != nullptr)
    {
        wxNumberEntryDialog dialog(this, "Scale", "Scale", "Divided by 10", 2, 1, 1000);
        int scale = 0;
        int delta = 0;

        if (dialog.ShowModal() == wxID_OK)
        {
            scale = dialog.GetValue();

            wxNumberEntryDialog dialog2(this, "Delta", "Delta", "Delta", 2, 1, 1000);
            if (dialog2.ShowModal() == wxID_OK)
            {
                delta = dialog2.GetValue();
                ApplyAlgorithm(f5, true, 3, scale, delta, CV_16S);
            }
        }
        return;
    }
}
