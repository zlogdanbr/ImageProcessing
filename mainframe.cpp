
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

    auto menuItemFileNew = menuFile->Append(wxID_NEW);
    menuItemFileNew->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));

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
    auto menuItemAlgo2 = menuAlgo->Append(ALGO2, "Algo\tCTRL+E", "algo");


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
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
    SetMenuBar(mainMenu);
   
    // -----------------------------------------------------------------------------
    //          Image
    // ------------------------------------------------------------------------------
    main_panel = new wxPanel(this, -1);
    staticBitmap1 = new wxStaticBitmap(main_panel, wxID_ANY, wxNullBitmap, { 20, 20 }, { 100, 50 });

    Centre();
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(    this, 
                                    wxEmptyString, 
                                    wxEmptyString, 
                                    "MyFile.bmp",
                                    "Image files (*.bmp)|*.bmp|All Files (*.*)|*.*", 
                                    wxFD_SAVE);

    if (saveFileDialog.ShowModal() == wxID_OK) 
    {
        wxString f = saveFileDialog.GetPath();
        if (images_map.SaveImage(f))
        {
            images_map.setFinalImage(f);
        }
    }
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(    this,
                                    wxEmptyString,
                                    wxEmptyString,
                                    wxEmptyString,
                                    "Image files (*.bmp)|*.bmp|All Files (*.*)|*.*",
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);


    openFileDialog.SetFilterIndex(0);
    if (openFileDialog.ShowModal() == wxID_OK)
    {     
        wxString path = wxString::Format("File = %s", openFileDialog.GetPath());
        if (images_map.setOriginalImage(path))
        {
            staticBitmap1->SetBitmap(wxBitmapBundle{ wxBitmap {images_map.original.GetFullPath(), wxBITMAP_TYPE_ANY} });
            staticBitmap1->SetWindowStyle(wxBORDER_SIMPLE);
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
    }

}


