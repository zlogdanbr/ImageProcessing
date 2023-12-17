
#include "mainframe.h"


MyFrame::MyFrame() :wxFrame{ nullptr, -1, "diMage", wxPoint(-1, -1) }
{
    SetClientSize(640, 480);
    wxInitAllImageHandlers();
    
    // ------------------------------------------------------------------------------  
    // menu   file
    // ------------------------------------------------------------------------------  
    auto menuFile = new wxMenu();

    auto menuItemFileOpen = menuFile->Append(wxID_OPEN);
    menuItemFileOpen->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));

    menuFile->Append(wxID_CLOSE, "Close\tCtrl+W");

    menuFile->AppendSeparator();

    auto menuItemFileSave = menuFile->Append(wxID_SAVE);
    menuItemFileSave->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU));

    menuFile->AppendSeparator();

    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    // ------------------------------------------------------------------------------  
    // menu   algos
    // ------------------------------------------------------------------------------  
    auto menuAlgo = new wxMenu();
    AddSubitemsToMenu(menuAlgo);

    // -----------------------------------------------------------------------------  
    // menu   help
    // -----------------------------------------------------------------------------
    auto menuHelp = new wxMenu();
    menuHelp->Append(wxID_ABOUT);

    // ----------------------------------------------------------------------------- 
    // Menu loaded
    // -----------------------------------------------------------------------------
    mainMenu->Append(menuFile, "&File");
    mainMenu->Append(menuAlgo, "&Tools");
    mainMenu->Append(menuHelp, "&Help");    

    // -----------------------------------------------------------------------------  
    // Menu Events
    // -----------------------------------------------------------------------------
    BinAllEvents();
    SetMenuBar(mainMenu);

    outxt.writeTo("Application initiated.\n");

    Centre();
}

void MyFrame::saveShowImage(Mat& img)
{
    if (img.empty() == false)
    {
        ImageHelper.setFinalImageOpenCV(img);
        ImageHelper.SetOriginalNew();
        ImageHelper.ShowFinal();
    }
    else
    {
        outxt.writeTo("Algorithm error\n");
    }
}

void MyFrame::OnAdjustContrast(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        // Mat adjustContrast(const Mat& img, int factor)
        info inf;
        inf.default_value = 50;
        inf.max = 100;
        inf.min = 1;
        inf.title = "Adjust Contrast";
        inf.default_value_string = "50";

        CSliderDialog dialog(this, inf);
        int scale = -90566000;

        dialog.ShowModal();
        scale = dialog.getValue();

        if (scale == -9056600)
        {
            return;
        }

        Mat clone = ImageHelper.getOrginalImageOpenCV();
        clone = adjustContrast(clone, scale);
        saveShowImage(clone);
    }
}

void MyFrame::OnAdjusyBright(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        // Mat adjustBrightness(const Mat& img, int factor)
        info inf;
        inf.default_value = 50;
        inf.max = 255;
        inf.min = -255;
        inf.title = "Adjust Brightness";
        inf.default_value_string = "50";

        CSliderDialog dialog(this, inf);
        int scale = -90566000;

        dialog.ShowModal();
        scale = dialog.getValue();

        if (scale == -9056600)
        {
            return;
        }

        Mat clone = ImageHelper.getOrginalImageOpenCV();
        clone = adjustBrightness(clone, scale);
        saveShowImage(clone);
    }
}

void MyFrame::OnAdjustGama(wxCommandEvent& event)
{    
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        //Mat adjustGama(const Mat & img, double gamma)
        info inf;
        inf.default_value = 50;
        inf.max = 100;
        inf.min = 1;
        inf.title = "Gamma Correction %";
        inf.default_value_string = "50";

        CSliderDialog dialog(this, inf);
        dialog.ShowModal();

        int value = -9056600;

        value = dialog.getValue();

        if (value == -9056600)
        {
            return;
        }

        double gamma = static_cast<double>(value) / 100;
        Mat clone = ImageHelper.getOrginalImageOpenCV();
        clone = adjustGama(clone, gamma);
        saveShowImage(clone);
    }
}

void MyFrame::OnAdjustThreshold(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        //Mat ApplyThreShold(const Mat & img, double _threshold)
        info inf;
        inf.default_value = 50;
        inf.max = 255;
        inf.min = 0;
        inf.title = "Threshold";
        inf.default_value_string = "50";

        CSliderDialog dialog(this, inf);
        dialog.ShowModal();

        int value = -9056600;

        value = dialog.getValue();

        if (value == -9056600)
        {
            return;
        }

        double threshold = static_cast<double>(value);

        Mat clone = ImageHelper.getOrginalImageOpenCV();
        clone = ApplyThreShold(clone, threshold);
        saveShowImage(clone);
    }
}


void MyFrame::onSelectRoi(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {

        Mat clone = ImageHelper.getOrginalImageOpenCV();

        wxRect sizeScreen = wxGetClientDisplayRect();
        int hs = sizeScreen.height;
        int ws = sizeScreen.width;

        clone = fitImageOnScreen(clone, ws, hs);
        Rect rect = selectROI("Final", clone, false);
        clone = Mat(clone, rect);
        clone = fitImageOnScreen(clone,ws, hs);
        if (clone.empty() == false)
        {
            ImageHelper.setFinalImageOpenCV(clone);
            outxt.writeTo("ROI selected.\n");
            ImageHelper.SetOriginalNew();
        }
        else
        {
            outxt.writeTo("Algorithm error\n");
        }
    }
    else
    {
        outxt.writeTo("Image not loaded\n");
    }
}

void MyFrame::onHelpFile(wxCommandEvent& event)
{
    std::stringstream os;
    os << "2023 Daniel Vasconcelos Gomes zlogdan.wordpress.com \n";
    outxt.writeInfo(os);
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName(wxTheApp->GetAppName());
    aboutInfo.SetVersion("1.0", "1.0.2");
    aboutInfo.SetDescription("Image Processing Tool");
    aboutInfo.SetWebSite("http://zlogdan.wordpress.com");
    aboutInfo.SetLicence("GNU GENERAL PUBLIC LICENSE GPLv3");
    wxArrayString Dev;
    wxString _me_ = "Daniel Vasconcelos Gomes";
    Dev.Add(_me_);
    aboutInfo.SetDevelopers(Dev);
    wxAboutBox(aboutInfo);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close();
}

void MyFrame::OnClose(wxCommandEvent& event)
{
    destroyAllWindows();
    ImageHelper.clearCache();
    ImageHelper.clean();
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    CSaveImage* saveFileCustom{ new CSaveImage(&ImageHelper,&outxt) };
    saveFileCustom->SaveFile(*this);
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    COpenImage* openFileCustom { new COpenImage(&ImageHelper,&outxt)};
    openFileCustom->OpenFile(*this);
}

void MyFrame::onCompare(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        Mat i1 = convertograyScale(ImageHelper.getOriginalInFact());
        Mat i2 = convertograyScale(ImageHelper.getOrginalImageOpenCV());
        i1.convertTo(i1, CV_8UC1);
        i2.convertTo(i2, CV_8UC1);

        Size s = i1.size();
        int w = s.width;
        int h = s.height;

        resize(i1, i1, Size(w/2, h/2), INTER_LINEAR);
        resize(i2, i2, Size(w/2, h/2), INTER_LINEAR);
        Mat clone;

        try
        {
            hconcat(i1, i2, clone);
            wxRect sizeScreen = wxGetClientDisplayRect();
            cv::namedWindow("Comparison", cv::WINDOW_NORMAL);
            clone = fitImageOnScreen(clone, sizeScreen.width, sizeScreen.height);
            imshow("Comparison", clone);
        }
        catch (...)
        {
            outxt.writeTo("Error. Cannot compare images.\n");
        }
    }

}

void MyFrame::onAllMenu(wxCommandEvent& event)
{   
    CInputDialog* InputDialog = new CInputDialog(this, &ImageHelper, &outxt);
    outxt.writeTo("Open Data Input dialog.\n");
    InputDialog->ShowModal();
}

void MyFrame::onCustomKernel(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        CGridInputDialog* MyDialog{ new CGridInputDialog(this)};
        MyDialog->setImageHelper(&ImageHelper);
        MyDialog->setLogs(&outxt);
        outxt.writeTo("Open Data Input dialog.\n");
        MyDialog->Show(true);

    }
    else
    {
        outxt.writeTo("Image not loaded\n");
    }

}

void MyFrame::onRevert(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        if (ImageHelper.revert() == false)
        {
            outxt.writeTo("Error, final image not loaded\n");
        }
    }
}