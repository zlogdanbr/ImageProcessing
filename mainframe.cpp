
#include "mainframe.h"
#include "detection.h"


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
   
    // -----------------------------------------------------------------------------
    //          Image

    outxt.writeTo("Application initiated.\n");

    Centre();
}

void MyFrame::onHelpFile(wxCommandEvent& event)
{
    std::stringstream os;
    os << "2023 Daniel Vasconcelos Gomes zlogdan.wordpress.com \n" ;
    outxt.writeInfo(os);
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

void MyFrame::onAllMenu(wxCommandEvent& event)
{
    outxt.writeTo("Ao menu all.\n");
    
    CInputDialog* InputDialog = new CInputDialog(this);
    InputDialog->setImageHelper(&ImageHelper);
    InputDialog->setLogs(&outxt);
    outxt.writeTo("Open Data Input dialog.\n");
    InputDialog->Show(true);

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

void MyFrame::onSumImage(wxCommandEvent& event)
{
    //if (ImageHelper.getOriginalImageInitiated() == true)
    //{
    //    if ( ImageHelper.SumImages() == false )
    //    {
    //        outxt.writeTo("Error, final image not loaded\n");
    //    }
    //}
}

void MyFrame::onSubImage(wxCommandEvent& event)
{
    //if (ImageHelper.getOriginalImageInitiated() == true)
    //{
    //    if ( ImageHelper.SubtractImages() == false )
    //    {
    //        outxt.writeTo("Error, final image not loaded\n");
    //    }
    //}
}

void MyFrame::onAdjustContrast(wxCommandEvent& event)
{
    //if (ImageHelper.getOriginalImageInitiated() == true)
    //{
    //    int contrast = getContrastDialog();

    //    if (contrast <= 0)
    //    {
    //        outxt.writeTo("Error, final image not loaded\n");
    //        return;
    //    }
    //    ImageHelper.AdjustContrast(contrast);
    //}
}

void MyFrame::onDisplayImage(wxCommandEvent& event)
{
    CImageCustomDialog* ImageDialog = new CImageCustomDialog(this);
    ImageDialog->Show(true);
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