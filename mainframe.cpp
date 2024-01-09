
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

void MyFrame::onImageDraw(wxCommandEvent& event)
{
    // Not working
    wxMessageBox("Not Implemented yet", "Error", wxOK | wxICON_ERROR);
    outxt.writeTo("Not implemented yet.\n");
}

void MyFrame::onApplySIFT(wxCommandEvent& event)
{
    CApplySift ImgSet(this,&outxt, -1, "Select Images", 2);
    ImgSet.ShowModal();
    if (ImgSet.IsoK)
    {
        ImgSet.doProcess();
    }
}

void MyFrame::onApplyHuh(wxCommandEvent& event)
{
    CMatchHuMomments ImgSet(this, &outxt, -1, "Select Images", 8);
    ImgSet.ShowModal();
    if (ImgSet.IsoK)
    {
        ImgSet.doProcess();
        wxString out = ImgSet.getInfo().c_str();
        outxt.writeTo( out );
        outxt.writeTo("End.");
    }
}

void MyFrame::onApplyTemplate(wxCommandEvent& event)
{
    CMatchTemplate ImgSet(this, &outxt, -1, "Select Images", 2);
    ImgSet.ShowModal();
    if (ImgSet.IsoK)
    {
        ImgSet.doProcess();
    }
}


void MyFrame::onApplyTemplateFull(wxCommandEvent& event)
{
    CMatchTemplateFull ImgSet(this, &outxt, -1, "Select Images", 8);
    ImgSet.ShowModal();
    if (ImgSet.IsoK)
    {
        ImgSet.doProcess();
    }
}


void MyFrame::onHelpFile(wxCommandEvent& event)
{
    std::stringstream os;
    os << "2023 Daniel Vasconcelos Gomes zlogdan.wordpress.com \n";
    outxt.writeInfo(os);
    wxAboutDialogInfo aboutInfo;
    aboutInfo.SetName(wxTheApp->GetAppName());
    aboutInfo.SetVersion("1.0", "1.0.3");
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
    destroyAllWindows();
    Close();
}

void MyFrame::OnClose(wxCommandEvent& event)
{
    destroyAllWindows();
    ImageHelper.clean();
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    CSaveImage* saveFileCustom{ new CSaveImage(&ImageHelper,&outxt) };
    saveFileCustom->SaveFile(*this);
    if (saveFileCustom != nullptr)
    {
        delete saveFileCustom;
    }
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    COpenImage* openFileCustom { new COpenImage(&ImageHelper,&outxt)};
    openFileCustom->OpenFile(*this);
    if (openFileCustom != nullptr)
    {
        delete openFileCustom;
    }
}

void MyFrame::onAllMenu(wxCommandEvent& event)
{   

    if (ImageHelper.getOriginalImageInitiated() == true)
    {
        CInputDialog* InputDialog = new CInputDialog(this, ImageHelper.getOrginalImageOpenCV());
        outxt.writeTo("Open Data Input dialog.\n");

        InputDialog->ShowModal();

        if (InputDialog->op_cancelled == false)
        {
            Mat out = InputDialog->getOutPutImage();

            if (out.empty() == false)
            {
                ImageHelper.setFinalImageOpenCV(out);
                ImageHelper.setOrginalImageOpenCV(out);
                showImage(out, "Final");
            }
        }
        else
        {
            showImage(ImageHelper.getOrginalImageOpenCV(), "Final");
        }

        if (InputDialog != nullptr)
        {
            delete InputDialog;
        }
    }
}
