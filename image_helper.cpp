#include "image_helper.h"
#include <wx/numdlg.h>
#include "image_util.h"


std::string convertWxStringToString(const wxString wsx)
{
    std::stringstream s;
    s << wsx;
    return s.str();
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

Mat CImageHelper::SetOriginalNew()
{
    Mat savefinal;
    savefinal = Final_ImageOpenCVFormat.clone();
    destroyAllWindows();
    clean();
    cv::namedWindow("Final", cv::WINDOW_NORMAL);
    setOrginalImageOpenCV(savefinal);
    original_initiated = true;
    return savefinal;
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
