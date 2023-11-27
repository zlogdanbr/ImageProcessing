#ifndef _IMAGE_HELPER_FUNC_
#define _IMAGE_HELPER_FUNC_
//--------------------------------------------------------------------------------------------------
// Helper class for converting images, manipulating them and storing
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#include <wx/wx.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/artprov.h>
#include <memory>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include "filesys.h"
#include "opcvwrapper.h"
#include "detection.h"
#include <functional>
#include<opencv2/highgui/highgui.hpp>

// setMouseCallback("Track", locator, NULL);//Mouse callback function on define window//

static void locator(int event, int x, int y, int flags, void* userdata) 
{ 
    //function to track mouse movement and click//
    if (event == EVENT_LBUTTONDOWN) { //when left button clicked//
        //cout << "Left click has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == EVENT_RBUTTONDOWN) { //when right button clicked//
        //cout << "Rightclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == EVENT_MBUTTONDOWN) { //when middle button clicked//
        //cout << "Middleclick has been made, Position:(" << x << "," << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE) { //when mouse pointer moves//
        //cout << "Current mouse position:(" << x << "," << y << ")" << endl;
    }
}


std::string convertWxStringToString(const wxString wsx);

class CImageHelper final
{
public:

    CImageHelper() = default;

    ~CImageHelper()
    {
        clean();
    }

    bool setOriginalImage(std::string& path)
    {
        original = path;
        original_initiated = file_exists(path);
        return original_initiated;
    }

    bool revert();

    std::string getOriginalImage() { return original; };

    Mat getOrginalImageOpenCV() const { return Original_ImageOpenCVFormat; };
    Mat getFinalImageOpenCV() const { return Final_ImageOpenCVFormat; };
    void setOrginalImageOpenCV(Mat& m) { Original_ImageOpenCVFormat = m; };
    void setFinalImageOpenCV(Mat& m) { Final_ImageOpenCVFormat = m; };
    bool getOriginalImageInitiated() const { return original_initiated; };

    void clean();
    bool SaveImage(std::string& Path);
    void setFinalGray(bool b) { final_isgray = b; };
    const bool getFinalGray() const { return final_isgray; };
    void SetOriginalNew();

    bool SumImages();
    bool SubtractImages();
 
    bool AdjustContrast(double scale);

private:

    CImageHelper(CImageHelper&) = delete;
    CImageHelper& operator=(CImageHelper&) = delete;
    Mat Final_ImageOpenCVFormat;
    Mat Original_ImageOpenCVFormat;
    bool original_initiated = false;
    bool final_isgray = false;
    std::string original = "";

    Mat savefinal;
    Mat saveOriginal;

    template<typename T1, typename T2>
    auto Plus(T1&& t1, T2&& t2) -> decltype(std::forward<T1>(t1) + std::forward<T2>(t2))
    {
        return std::forward<T1>(t1) + std::forward<T2>(t2);
    }

    template<typename T1, typename T2>
    auto Sub(T1&& t1, T2&& t2) -> decltype(std::forward<T1>(t1) - std::forward<T2>(t2))
    {
        return std::forward<T1>(t1) - std::forward<T2>(t2);
    }

    template<typename T1, typename T2, typename F>
    auto GenericFunction(T1&& t1, T2&& t2,F&& f) -> decltype(std::forward<T2>(t2))
    {
        return F(t1);
    }

};
#endif
//--------------------------------------------------------------------------------------------------