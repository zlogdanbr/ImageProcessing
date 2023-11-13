
#include "mainframe.h"


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

    menuFile->AppendSeparator();

    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    // ------------------------------------------------------------------------------  
    // menu   algos
    // ------------------------------------------------------------------------------  
    auto menuAlgo = new wxMenu();
    auto menuItemNode= menuAlgo->Append(ALGO_NODE_REC, "Find Contours\tCTRL+F", "Find Contours");
    auto menuItemGray = menuAlgo->Append(ALGO_GRAY_C,"Gray Scale\tCTRL+G", "Converts to Gray");
    auto menuItemHist = menuAlgo->Append(ALGO_EQUALIZE, "Equalize\tCTRL+E", "Equalize image");
    auto menuItemLaplacian = menuAlgo->Append(ALGO_LAPLACIAN, "Laplacian\tCTRL+L", "Laplacian");
    menuAlgo->AppendSeparator();
    auto menuItemBlur33 = menuAlgo->Append(ALGO_BLUR33, "Blur Kernel Size 3\tCTRL+B", "Blur Kernel Size 3");
    auto menuItemBlur55 = menuAlgo->Append(ALGO_BLUR55, "Blur Kernel Size 5\tCTRL+T", "Blur Kernel Size 5");
    auto menuItemBlurGaussian = menuAlgo->Append(ALGO_GAUSSIAN, "Gaussian Kernel Size 5\tCTRL+A", "Gaussian Kernel Size 5");
    auto menuItemMedian = menuAlgo->Append(ALGO_MEDIAN, "Median Filter\tCTRL+M", "Median Filter Size 5");
    menuAlgo->AppendSeparator();
    auto menuFlipH= menuAlgo->Append(FLIP_H, "Flip Image Horizontal", "lip Image Horizontal");
    auto menuFlipV = menuAlgo->Append(FLIP_V, "Flip Image Vertical", "Flip Image Vertical");
    auto menuFlip = menuAlgo->Append(FLIP_B, "Flip Image", "Flip Image");
    auto menuSum = menuAlgo->Append(SUMIMG, "Sum Images", "Summing Images");

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
    Bind(wxEVT_MENU, &MyFrame::OnNoduleRec, this, ALGO_NODE_REC);
    Bind(wxEVT_MENU, &MyFrame::OnDoGrayScale, this, ALGO_GRAY_C);
    Bind(wxEVT_MENU, &MyFrame::OnDoEqualize, this, ALGO_EQUALIZE);
    Bind(wxEVT_MENU, &MyFrame::OnDoLaplacian, this, ALGO_LAPLACIAN); 
    Bind(wxEVT_MENU, &MyFrame::onImageBlurKernel33, this, ALGO_BLUR33);
    Bind(wxEVT_MENU, &MyFrame::onImageBlurKernel55, this, ALGO_BLUR55);
    Bind(wxEVT_MENU, &MyFrame::onMedian, this, ALGO_MEDIAN);
    Bind(wxEVT_MENU, &MyFrame::onGaussian, this, ALGO_GAUSSIAN);
    Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE); 
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MyFrame::onFlipV, this, FLIP_H);
    Bind(wxEVT_MENU, &MyFrame::onFlipH, this, FLIP_V);
    Bind(wxEVT_MENU, &MyFrame::onFlipA, this, FLIP_B);
    Bind(wxEVT_MENU, &MyFrame::onSumImages, this, SUMIMG);
    


    SetMenuBar(mainMenu);
   
    // -----------------------------------------------------------------------------
    //          Image

    outxt.writeTo("Application initiated...\n");

    Centre();
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
            //auto name_final = ImageHelper.getOriginalImage().GetName();
            //auto path = ImageHelper.getOriginalImage().GetPath();
            //auto tosave = path + "\\" + name_final + "_proc_" + ".jpg";

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
                                    "Image files (*.jpg)|*.jpg|All Files (*.*)|*.*",
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
        Mat out = n.getEdgesImg();
        ImageHelper.setFinalImageOpenCV(out);
        showImage(ImageHelper.getFinalImageOpenCV(), "Final");
        outxt.writeTo("Algorithm applied correctly\n");
        ImageHelper.setFinalImage(spath);
    }
    else
    {
        outxt.writeTo("Algorithm error\n");
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

template<typename F>
void
MyFrame::ApplyAlgorithm(F& f, bool Gray, int kernel_size)
{
    auto spath = ImageHelper.getOriginalImage();
    Mat img;
    Mat out;
    ImageHelper.setFinalGray(Gray);

    if (loadImage(spath, img) == true)
    {
        out = f(img, kernel_size);
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

void MyFrame::OnDoGrayScale(wxCommandEvent& event)
{
    ApplyAlgorithm(convertograyScale, true);    
}

void MyFrame::OnDoEqualize(wxCommandEvent& event)
{
    ApplyAlgorithm(equalizeColorImage, false);
}

void MyFrame::OnDoLaplacian(wxCommandEvent& event)
{
    ApplyAlgorithm(laplacian, false);
}

void MyFrame::onImageBlurKernel33(wxCommandEvent& event)
{
    ApplyAlgorithm(blurImageSmooth, false, 3);
}

void MyFrame::onImageBlurKernel55(wxCommandEvent& event)
{
    ApplyAlgorithm(blurImageSmooth, false, 5);
}

void MyFrame::onGaussian(wxCommandEvent& event)
{
    ApplyAlgorithm(GaussianImageSmooth, false, 5);
}

void MyFrame::onMedian(wxCommandEvent& event)
{
    ApplyAlgorithm(MedianImageSmooth, false, 3);
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

void MyFrame::onSumImages(wxCommandEvent& event)
{
    if (ImageHelper.getOriginalImageInitiated() && ImageHelper.getFinallImageInitiated())
    {
        Mat out = SumImages(ImageHelper.getOrginalImageOpenCV(), ImageHelper.getFinalImageOpenCV());

        if (out.empty() == false)
        {
            ImageHelper.setFinalImageOpenCV(out);
            showImage(ImageHelper.getFinalImageOpenCV(), "Final");
            outxt.writeTo("Algorithm applied correctly\n");
            ImageHelper.setFinalGray(true);
        }
        else
        {
            outxt.writeTo("Error\n");
        }
    }
    else
    {
        outxt.writeTo("Images not loaded\n");
    }
}
