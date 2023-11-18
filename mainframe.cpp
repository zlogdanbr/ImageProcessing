
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

    // ------------------------------------------------------------------------------  
    // menu   custom
    // ------------------------------------------------------------------------------  
    auto menuCustom = new wxMenu();
    auto menuItemNode = menuCustom->Append(ALGO_NODE_REC, "Find Contours\tCTRL+F", "Find Contours");
    auto menuCtsKernl = menuCustom->Append(CUSTKERNEL, "Apply customized kernel", "Input custom kernel");
    auto menuDetecFaces = menuCustom->Append(FACE_DETEC, "Find Faces", "Find Faces");

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
    mainMenu->Append(menuCustom, "A&dvanced");
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
    outxt.writeTo("Help instructions loaded.\n");
    os << "2023 Daniel Vasconcelos Gomes\n" ;
    os << "zlogdan.wordpress.com\n";
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

        wxFileDialog saveFileDialog(this, wxEmptyString, wxEmptyString, "MyFile.jpg", "Text Files (*.jpg)|*.jpg|All Files (*.*)|*.*", wxFD_SAVE);
        if (saveFileDialog.ShowModal() == wxID_OK) 
        {

            wxString spath = saveFileDialog.GetPath();
            std::string path = convertWxStringToString(spath);

            if (ImageHelper.SaveImage(path))
            {
                outxt.writeTo("Image sucessfully saved as:\n");
                outxt.writeTo(spath + "\n");
                destroyAllWindows();
                ImageHelper.clean();
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
                ImageHelper.setOrginalImageOpenCV(img);
                showImage(ImageHelper.getOrginalImageOpenCV(), "Original");
                outxt.writeTo("Image loaded correctly\n");
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

void MyFrame::OnNoduleRec(wxCommandEvent& event)
{
    auto spath = ImageHelper.getOriginalImage();
    ImageHelper.setFinalGray(true);
    NoduleRec n{ spath };

    if (n.ErrorInOriginalLoading() == false)
    {
        n.findContornos(155);
        n.HighlightRoi();
        Mat out2 = n.getEdgesImg();
        Mat out = n.getFinalImg();
        if (out.empty())
        {
            outxt.writeTo("Error applying algorithm\n");
            return;
        }
        ImageHelper.setFinalImageOpenCV(out2);
        showImage(out, "Final");
        showImage(ImageHelper.getFinalImageOpenCV(), "Final2");
        outxt.writeTo("Algorithm applied correctly\n");
        ImageHelper.setFinalImage(spath);
    }
    else
    {
        outxt.writeTo("Error loading image\n");
    }

}

template<typename F>
void
MyFrame::ApplyAlgorithm(F& f, bool Gray)
{
    auto spath = ImageHelper.getOriginalImage();
    Mat img;
    Mat out;
    ImageHelper.setFinalGray(Gray);

    if (loadImage(spath, img) == true)
    {
        out = f(img);
        if (out.empty() == false)
        {
            ImageHelper.setFinalImageOpenCV(out);
            showImage(ImageHelper.getFinalImageOpenCV(), "Final");
            outxt.writeTo("Algorithm applied correctly\n");
            ImageHelper.setFinalGray(true);
            ImageHelper.setFinalImage(spath);
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

void MyFrame::onThreshold(wxCommandEvent& event)
{
    if (ImageHelper.getFinallImageInitiated() == true)
    {
        Mat out  = ImageHelper.getFinalImageOpenCV();
        if (out.empty() == false)
        {
            out = ApplyThreShold(out, 0.0001);
            ImageHelper.setFinalImageOpenCV(out);
            showImage(ImageHelper.getFinalImageOpenCV(), "Final");
            outxt.writeTo("Algorithm applied correctly\n");
            ImageHelper.setFinalGray(true);
            return;
        }
    }
    outxt.writeTo("Final image not yet set\n");
}

void MyFrame::onFaces(wxCommandEvent& event)
{
    // It should work but only God knows why it is not
    try
    {
        ApplyAlgorithm(detectEyes, false);
    }
    catch (...)
    {
        outxt.writeTo("Only God knows why it does not work.\n");
        outxt.writeTo("Maybe it is because of the OS.\n");
        outxt.writeTo("Please, try with Windows 10.\n");
    }
    
}

void MyFrame::AddSubitemsToMenu(wxMenu* menuAlgo)
{        
    auto menumenuALL = menuAlgo->Append(ONE_ID_TO_ALL, "Base Algorithms", "Base Algorithms");
    auto menuThresholdL = menuAlgo->Append(THRESHOLD_FINAL, "Apply Threshold", "Apply Threshold");
    menuAlgo->AppendSeparator();
    auto menuFlipH = menuAlgo->Append(FLIP_H, "Flip Image Horizontal", "lip Image Horizontal");
    auto menuFlipV = menuAlgo->Append(FLIP_V, "Flip Image Vertical", "Flip Image Vertical");
    auto menuFlip = menuAlgo->Append(FLIP_B, "Flip Image", "Flip Image");  
    
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

void MyFrame::onFlipV(wxCommandEvent& event)
{
    ApplyAlgorithm(flipImageHorizontal, false);
}

void MyFrame::onFlipH(wxCommandEvent& event)
{
    ApplyAlgorithm(flipImageVertical, false);
}

void MyFrame::onFlipA(wxCommandEvent& event)
{
    ApplyAlgorithm(flipImage, false);
}

