#include "childframes.h"

std::function<Mat(Mat)> 
CInputDialog::getAlgoFunctionSimple(wxString key)
{
    if (fsimple.find(key) != fsimple.end())
    {
        return fsimple[key];
    }
    return nullptr;
}

std::function<Mat(Mat, int)> 
CInputDialog::getAlgoFunctionMore(wxString key)
{
    if (fmore.find(key) != fmore.end())
    {
        return fmore[key];
    }
    return nullptr;
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
    fsimple["Convert to Gray Scale"] = convertograyScale;
    fsimple["Equalize Gray Scale Image"] = equalizeGrayImage;
    fsimple["Equalize Color Scale Image"] = equalizeColorImage;
    fsimple["Apply Laplacian"] = laplacian;
    fsimple["Detect Corners"] = detectCornersHarris;
    fsimple["Detect features"] = fastDetectKeyPoints;
    fsimple["Working Algorithm"] = workingAlgorithm;
    fsimple["Detect Corners"] = detectCornersHarris;
    fsimple["Detect features"] = fastDetectKeyPoints;
    fsimple["Flip Image Horizontally"] = flipImageHorizontal;
    fsimple["Flip Image Vertically"] = flipImageVertical;
    fsimple["Flip Image"] = flipImage;
    fsimple["Apply Threshold"] = ApplyThreShold;
    fsimple["Canny"] = ApplyCanny;
    fmore["Blur Image"] = blurImageSmooth;
    fmore["Gaussian"] = GaussianImageSmooth;
    fmore["Median"] = MedianImageSmooth;
    fmore["Sobel"] = ApplySobel;

    fsimple["Hough Transform"] = ApplyHoughTransformRegular;
    fsimple["Hough Transform Probabilistic"] = ApplyHoughTransformReProbabilistic;

    comboBox1->Append("Working Algorithm");
    comboBox1->Append("Convert to Gray Scale");
    comboBox1->Append("Equalize Gray Scale Image");
    comboBox1->Append("Equalize Color Scale Image");
    comboBox1->Append("Apply Laplacian");
    comboBox1->Append("Blur Image");
    comboBox1->Append("Gaussian");
    comboBox1->Append("Median");
    comboBox1->Append("Detect Corners");
    comboBox1->Append("Detect features");    
    comboBox1->Append("Apply Threshold");
    comboBox1->Append("Sobel");
    comboBox1->Append("Canny");
    comboBox1->Append("Sobel");
    comboBox1->Append("Hough Transform");
    comboBox1->Append("Hough Transform Probabilistic");
    comboBox1->Append("Flip Image Horizontally");
    comboBox1->Append("Flip Image Vertically");
    comboBox1->Append("Flip Image");

}

void CInputDialog::DoFunction()
{
    outxt->writeTo("Applying algorithm... please wait...\n");
    wxString opt = getSelectionText();

    Function1Parameter f1 = getAlgoFunctionSimple(opt);

    if (f1 == nullptr)
    {
        Function2Parameter f2 = getAlgoFunctionMore(opt);
        if (f2 == nullptr)
        {
            outxt->writeTo("Error while loading algos.\n");
            stop = true;
            return;
        }

        ApplyAlgorithm(f2, true, 5);
    }
    else
    {
        ApplyAlgorithm(f1, true);
    }
    stop = true;
}

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
