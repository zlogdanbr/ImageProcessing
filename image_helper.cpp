#include "image_helper.h"
#include <wx/numdlg.h>

std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
}

// TODO
bool CImageHelper::SumImages()
{
    return true;
}

// TODO
bool CImageHelper::SubtractImages()
{
    return true;
}

// TODO
bool CImageHelper::AdjustContrast(double scale)
{
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

bool CImageHelper::revert()
{
    if (cache.size() == 0)
    {
        return false;
    }

    Mat saved = cache.front();
    cache.pop();

    destroyAllWindows();
    clean();

    setOrginalImageOpenCV(saved);
    original_initiated = true;

    showImage(saved, "Final");

    return true;

}


void CImageHelper::addImageToCache(Mat& mat)
{
    cache.push(mat);
}

void CImageHelper::SetOriginalNew()
{
    Mat savefinal;

    savefinal = Final_ImageOpenCVFormat.clone();

    destroyAllWindows();
    clean();

    setOrginalImageOpenCV(savefinal);
    original_initiated = true;
    cache.push(savefinal);

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
