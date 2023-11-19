
#include "mainframe.h"
#include "detection.h"

MyFrame::MyFrame() :wxFrame{ nullptr, -1, "Image Processing", wxPoint(-1, -1) }
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
    mainMenu->Append(menuAlgo, "&Algorithms");
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
    ImageHelper.clean();
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() == true)
    {

        wxFileDialog saveFileDialog(    this, 
                                        wxEmptyString, 
                                        wxEmptyString, 
                                        "MyFile.jpg", "Text Files (*.jpg)|*.jpg|All Files (*.*)|*.*",
                                        wxFD_SAVE);
        if (saveFileDialog.ShowModal() == wxID_OK) 
        {

            wxString spath = saveFileDialog.GetPath();
            std::string path = convertWxStringToString(spath);

            if (ImageHelper.SaveImage(path))
            {
                outxt.writeTo("Image sucessfully saved as:\n");
                outxt.writeTo(spath + "\n");
            }
            else
            {
                outxt.writeTo("Error saving image.\n");
            }
        }
        else
        {
            outxt.writeTo("Error saving image.\n");
            return;
        }
    }
    else
    {
        outxt.writeTo("Image has not been loaded.\n");
    }
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(    this,
                                    wxEmptyString,
                                    wxEmptyString,
                                    wxEmptyString,
                                    "jpg and tif files(*.jpg; *.tif) | *.jpg; *.tif",
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    ImageHelper.clean();
    openFileDialog.SetFilterIndex(0);
    if (openFileDialog.ShowModal() == wxID_OK)
    {     
        wxString path =  openFileDialog.GetPath();
        std::string spath = convertWxStringToString(path);
        if (ImageHelper.setOriginalImage(spath))
        {
            Mat img;
            if (loadImage(spath, img) == true)
            {
                ImageHelper.clean();
                ImageHelper.setOrginalImageOpenCV(img);
                showImage(ImageHelper.getOrginalImageOpenCV(), "Original");
                outxt.writeTo("Image loaded correctly\n");
                ImageHelper.setOriginalImage(spath);
            }
            else
            {
                outxt.writeTo("Error loading Image\n");
            }
        }
        else
        {
            outxt.writeTo("Error loading Image\n");
        }
    }
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

