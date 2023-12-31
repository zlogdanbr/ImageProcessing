#include "childframes.h"

COpenImage::COpenImage(CImageHelper* imghelper, CWriteLogs* outxt) :imghelper{ imghelper }, outxt{ outxt }
{

}

void COpenImage::OpenFile(wxFrame& frame)
{
    wxFileDialog openFileDialog(&frame,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        //"Text Files (*.txt)|*.txt|All Files (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        "jpg and tif files(*.jpg; *.tif)| *.jpg; *.tif|All Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    imghelper->clean();
    openFileDialog.SetFilterIndex(0);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString path = openFileDialog.GetPath();
        std::string spath = convertWxStringToString(path);
        if (imghelper->setOriginalImage(spath))
        {
            Mat img;
            if (loadImage(spath, img) == true)
            {
                imghelper->clean();
                destroyAllWindows();
                imghelper->setOrginalImageOpenCV(img);                
                outxt->writeTo(path + "\n");
                outxt->writeTo("Image loaded correctly\n");
                imghelper->setOrginalImageOpenCV(img);
                imghelper->setOriginalImage(spath);
                imghelper->setOriginalInfact(img);
                showImage(img, "Original");
            }
            else
            {
                outxt->writeTo("Error loading Image\n");
            }
        }
        else
        {
            outxt->writeTo("Error loading Image\n");
        }
    }
}

CSaveImage::CSaveImage(CImageHelper* imghelper, CWriteLogs* outxt) :imghelper{ imghelper }, outxt{ outxt }
{

}

void CSaveImage::SaveFile(wxFrame& frame)
{
    if (imghelper->getOriginalImageInitiated() == true)
    {

        wxFileDialog saveFileDialog(&frame,
            wxEmptyString,
            wxEmptyString,
            "MyFile.jpg", "Text Files (*.jpg)|*.jpg|All Files (*.*)|*.*",
            wxFD_SAVE);
        if (saveFileDialog.ShowModal() == wxID_OK)
        {

            wxString spath = saveFileDialog.GetPath();
            std::string path = convertWxStringToString(spath);

            if (imghelper->SaveImage(path))
            {
                outxt->writeTo("Image sucessfully saved as:\n");
                outxt->writeTo(spath + "\n");
            }
            else
            {
                outxt->writeTo("Error saving image.\n");
            }
        }
        else
        {
            outxt->writeTo("Error saving image.\n");
            return;
        }
    }
    else
    {
        outxt->writeTo("Image has not been loaded.\n");
    }
}