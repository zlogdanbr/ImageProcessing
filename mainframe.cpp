
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
    
    if (InputDialog->ShowModal() == wxID_CANCEL)
    {
        outxt.writeTo("Apply Data Input dialog.\n");
    }

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