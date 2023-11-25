#include "image_helper.h"
#include <wx/numdlg.h>

std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}
bool CImageHelper::SumImages()
{
    savefinal = Final_ImageOpenCVFormat.clone();

    if (savefinal.empty())
    {
        return false;
    }

    saveOriginal = Original_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    Mat final = Plus(savefinal,saveOriginal);
    setOrginalImageOpenCV(final);
    original_initiated = true;

    showImage(saveOriginal, "Original");
    showImage(savefinal, "Final");

    return true;
}

bool CImageHelper::SubtractImages()
{
    savefinal = Final_ImageOpenCVFormat.clone();

    if (savefinal.empty())
    {
        return false;
    }

    saveOriginal = Original_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    Mat final = Sub(savefinal, saveOriginal);

    setOrginalImageOpenCV(final);
    original_initiated = true;

    showImage(saveOriginal, "Original");
    showImage(savefinal, "Final");

    return true;
}

void CImageHelper::clean()
{
    original_initiated = false;

    if (Final_ImageOpenCVFormat.empty() == false)
    {
        Final_ImageOpenCVFormat.deallocate();
    }
    if (Original_ImageOpenCVFormat.empty() == false)
    {
        Original_ImageOpenCVFormat.deallocate();
    }
}

bool CImageHelper::AdjustContrast(double scale)
{
    savefinal = Final_ImageOpenCVFormat.clone();

    if (savefinal.empty())
    {
        return false;
    }

    saveOriginal = Original_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    savefinal = (scale/static_cast<double>(10)) * saveOriginal;

    setOrginalImageOpenCV(savefinal);
    original_initiated = true;

    showImage(saveOriginal, "Original");
    showImage(savefinal, "Final");

    return true;
}

bool CImageHelper::revert()
{
    savefinal = Final_ImageOpenCVFormat.clone();

    if (savefinal.empty() || saveOriginal.empty())
    {
        return false;
    }

    destroyAllWindows();
    clean();

    setOrginalImageOpenCV(saveOriginal);
    showImage(saveOriginal, "Original");    
    original_initiated = true;

    savefinal.deallocate();

    return true;

}
void CImageHelper::SetOriginalNew()
{
    savefinal = Final_ImageOpenCVFormat.clone();
    saveOriginal = Original_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    setOrginalImageOpenCV(savefinal);
    original_initiated = true;

    showImage(saveOriginal, "Original");
    showImage(savefinal, "Final");

}

bool CImageHelper::SaveImage(std::string& Path)
{
    wxString s = convertWxStringToString(Path);
    Mat final_image = getFinalImageOpenCV();

    std::string spath = convertWxStringToString(Path);        
    if (saveImage(spath, final_image))
    {
        return true;
    }
    return false;

}
