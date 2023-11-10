
#include "mainframe.h"


std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}


bool CImageHelper::SaveImage(wxString& Path)
{

    if (getFinalGray() == false)
    {
        wxImage ImgToSave;
        if (convertOpenCVMatToWxImage(Final_ImageOpenCVFormat, ImgToSave))
        {
            return ImgToSave.SaveFile(Path);
        }
        return false;
    }
    else
    {
        std::string spath = convertWxStringToString(Path);
        Mat final = getFinalImageOpenCV();
        if (saveImage(spath, final))
        {
            return true;
        }
        return false;
    }

}

bool CImageHelper::convertOpenCVMatToWxImage(Mat& cvImg, wxImage& wxImg) const
{
    try
    {
        // data dimension
        int w = cvImg.cols;
        int h = cvImg.rows;
        int size = w * h * 3 * sizeof(unsigned char);

        // allocate memory for internal wxImage data
        unsigned char* wxData = (unsigned char*)malloc(size);

        // the matrix stores BGR image for conversion
        Mat cvRGBImg = Mat(h, w, CV_8UC3, wxData);
        switch (cvImg.channels())
        {
            case 3: // 3-channel case: swap R&B channels
            {
                int mapping[] = { 0,2,1,1,2,0 }; // CV(BGR) to WX(RGB)
                mixChannels(&cvImg, 1, &cvRGBImg, 1, mapping, 3);
            } break;

            default:
            {
            }
        }

        wxImg.Destroy(); // free existing data if there's any
        wxImg = wxImage(w, h, wxData);
    }
    catch (...)
    {
        return false;
    }

    return true;
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

    menuFile->AppendSeparator();

    auto menuItemFileQuit = menuFile->Append(wxID_EXIT);
    menuItemFileQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU));

    // ------------------------------------------------------------------------------  
    // menu   algos
    // ------------------------------------------------------------------------------  
    auto menuAlgo = new wxMenu();
    auto menuItemNode= menuAlgo->Append(ALGO_NODE_REC, "Segmentation\tCTRL+F", "Segments Image");
    auto menuItemGray = menuAlgo->Append(ALGO_GRAY_C,"Gray Scale\tCTRL+G", "Converts to Gray");
    auto menuItemHist = menuAlgo->Append(ALGO_EQUALIZE, "Equalize\tCTRL+E", "Equalize image");
    auto menuItemLaplacian = menuAlgo->Append(ALGO_LAPLACIAN, "Laplacian\tCTRL+L", "Laplacian");
    auto menuItemBlur33 = menuAlgo->Append(ALGO_BLUR33, "Blur Kernel Size 3\tCTRL+B", "Blur Kernel Size 3");
    auto menuItemBlur55 = menuAlgo->Append(ALGO_BLUR55, "Blur Kernel Size 5\tCTRL+T", "Blur Kernel Size 5");
    auto menuItemBlurGaussian = menuAlgo->Append(ALGO_GAUSSIAN, "Gaussian Kernel Size 5\tCTRL+A", "Gaussian Kernel Size 5");
    auto menuItemMedian = menuAlgo->Append(ALGO_MEDIAN, "Median Filter\tCTRL+M", "Median Filter Size 5");

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
    Bind(wxEVT_MENU, &MyFrame::OnClose, this, wxID_CLOSE); 
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
    SetMenuBar(mainMenu);
   
    // -----------------------------------------------------------------------------
    //          Image
    // ------------------------------------------------------------------------------
    textCtrl->AppendText("Application initiated...\n");

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
        auto name_final = ImageHelper.getOriginalImage().GetName();
        auto path = ImageHelper.getOriginalImage().GetPath();
        auto tosave = path + "\\" + name_final + "_proc_" + ".jpg";
        if (ImageHelper.SaveImage(tosave))
        {
            textCtrl->AppendText("Image sucessfully saved as:\n");
            textCtrl->AppendText(tosave + "\n");
        }
        else
        {
            textCtrl->AppendText("Error saving image.\n");
        }    
    }
    else
    {
        textCtrl->AppendText("Image has not been loaded.\n");
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
        if (ImageHelper.setOriginalImage(path))
        {
            Mat img;
            if (loadImage(spath, img) == true)
            {
                ImageHelper.setOrginalImageOpenCV(img);
                showImage(ImageHelper.getOrginalImageOpenCV(), "Original");
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

void MyFrame::OnNoduleRec(wxCommandEvent& event)
{
    auto path = ImageHelper.getOriginalImage().GetFullPath();
    std::string spath = convertWxStringToString(path);  
    ImageHelper.setFinalGray(false);
    NoduleRec n{ spath };

    if (n.ErrorInOriginalLoading() == false)
    {
        n.findContornos(1);
        n.HighlightRoi();
        Mat out = n.getFinalImg();
        ImageHelper.setFinalImageOpenCV(out);
        showImage(ImageHelper.getFinalImageOpenCV(), "Final");
        textCtrl->AppendText("Algorithm applied correctly\n");
    }
    else
    {
        textCtrl->AppendText("Algorithm error\n");
    }

}

template<typename F>
void
MyFrame::ApplyAlgorithm(F& f, bool Gray)
{
    auto path = ImageHelper.getOriginalImage().GetFullPath();
    std::string spath = convertWxStringToString(path);
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
            textCtrl->AppendText("Algorithm applied correctly\n");
            ImageHelper.setFinalGray(true);
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

template<typename F>
void
MyFrame::ApplyAlgorithm(F& f, bool Gray, int kernel_size)
{
    auto path = ImageHelper.getOriginalImage().GetFullPath();
    std::string spath = convertWxStringToString(path);
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
            textCtrl->AppendText("Algorithm applied correctly\n");
            ImageHelper.setFinalGray(true);
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
