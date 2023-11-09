
#include "mainframe.h"


std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}

MyFrame::MyFrame():wxFrame(NULL, -1, "My SkeletonApp", wxPoint(-1, -1))
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

    auto menuItemFileSaveAs = menuFile->Append(wxID_SAVEAS);
    menuItemFileSaveAs->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));

    menuFile->AppendSeparator();

    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    // ------------------------------------------------------------------------------  
    // menu   algos
    // ------------------------------------------------------------------------------  

    auto menuAlgo = new wxMenu();
    auto menuItemAlgo1= menuAlgo->Append(ALGO1, "Segmentation\tCTRL+F", "Segments Image");
    auto menuItemAlgo2 = menuAlgo->Append(ALGO2,"Gray Scale\tCTRL+G", "Converts to Gray");


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
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MyFrame::OnAlgo1, this, ALGO1);
    Bind(wxEVT_MENU, &MyFrame::OnAlgo2, this, ALGO2);
    SetMenuBar(mainMenu);
   
    // -----------------------------------------------------------------------------
    //          Image
    // ------------------------------------------------------------------------------
    textCtrl->AppendText("Init application\n");

    Centre();
}


void MyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(    this,
                                    wxEmptyString,
                                    wxEmptyString,
                                    wxEmptyString,
                                    "Image files (*.bmp)|*.bmp|All Files (*.*)|*.*",
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    images_map.clean();
    openFileDialog.SetFilterIndex(0);
    if (openFileDialog.ShowModal() == wxID_OK)
    {     
        wxString path =  openFileDialog.GetPath();
        std::string spath = convertWxStringToString(path);
        if (images_map.setOriginalImage(path))
        {
            Mat img;
            if (loadImage(spath, img) == true)
            {
                images_map.Original_ImageOpenCVFormat = img;
                showImage(images_map.Original_ImageOpenCVFormat, "");
                images_map.Original_ImageOpenCVFormat.deallocate();
                textCtrl->AppendText("Image loaded correctly\n");
            }
            else
            {
                textCtrl->AppendText("Error loading Image\n");
            }
        }
        else
        {
            textCtrl->AppendText("Error loading Image\n");
        }
    }
}

void MyFrame::OnAlgo1(wxCommandEvent& event)
{
    auto path = images_map.original.GetFullPath();
    std::string spath = convertWxStringToString(path);  

    NoduleRec n{ spath };

    if (n.ErrorInOriginalLoading() == false)
    {
        n.findContornos(1);
        n.HighlightRoi();
        images_map.Final_ImageOpenCVFormat = n.getFinalImg();
        showImage(images_map.Final_ImageOpenCVFormat, "");
        textCtrl->AppendText("Algorithm applied correctly\n");
    }
    else
    {
        textCtrl->AppendText("Algorithm error\n");
    }

}

void MyFrame::OnAlgo2(wxCommandEvent& event)
{
    auto path = images_map.original.GetFullPath();
    std::string spath = convertWxStringToString(path);
    Mat img;
    Mat out;

    if ( loadImage(spath, img) == true)
    {
        out = convertograyScale(img);
        if (out.empty() == false)
        {
            images_map.Final_ImageOpenCVFormat = out;
            showImage(images_map.Final_ImageOpenCVFormat, "");
            textCtrl->AppendText("Algorithm applied correctly\n");
        }
        else
        {
            textCtrl->AppendText("Algorithm error\n");
        }
    }
    else
    {
        textCtrl->AppendText("Image not loaded\n");
    }
    

}


