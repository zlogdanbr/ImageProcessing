//--------------------------------------------------------------------------------------------------
// Declaration of class interfaces for dialogs used in the application
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------
#ifndef _CHILD_FRAME_DEFS_
#define _CHILD_FRAME_DEFS_

#include <wx/dc.h>
#include <wx/grid.h>
#include <wx/wx.h>
#include <wx/progdlg.h>
#include <wx/numdlg.h>
#include <wx/aboutdlg.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <functional>
#include <thread>
#include "image_helper.h"
#include "opcvwrapper.h"
#include "logs.h"
#include "filesys.h"
#include "image_util.h"

using namespace image_util;

class COpenImage final
{
public:

    COpenImage(CImageHelper* imghelper, CWriteLogs* outxt);
    ~COpenImage()
    {

    }

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };
    void OpenFile(wxFrame& frame);

private:
    COpenImage(COpenImage&) = delete;
    COpenImage& operator=(COpenImage&) = delete;

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
};

class CSaveImage final
{
public:

    CSaveImage(CImageHelper* imghelper, CWriteLogs* outxt);
    ~CSaveImage()
    {

    }

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };
    void SaveFile(wxFrame& frame);

private:
    CSaveImage(CSaveImage&) = delete;
    CSaveImage& operator=(CSaveImage&) = delete;

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
};

class CInputDialogBase  : public wxFrame 
{
public:

    CInputDialogBase(wxFrame* parent, wxString name);
    ~CInputDialogBase()
    {

    }

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };

    wxFrame* _parent = nullptr;

protected:

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
    std::string  setPath(bool Gray);
    void  setFinalImg(Mat& out);   
    virtual void setControlslayout() = 0;

};


class CGridInputDialog final : public CInputDialogBase
{
public:

    CGridInputDialog(wxFrame* parent);
    ~CGridInputDialog()
    {

    }

private:

    //--------------------------------------------------------------
    // Components
    //--------------------------------------------------------------
    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1{ new wxPanel(basePanel) };
    wxPanel* panel2{ new wxPanel(basePanel, -1) };

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxVERTICAL) };
    wxBoxSizer* hbox1{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* hbox2{ new wxBoxSizer(wxHORIZONTAL) };

    wxButton* button1{ new wxButton(panel1, wxID_ANY, "OK")};
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel")};
    wxButton* button3{ new wxButton(panel1, wxID_ANY, "Clear")};
    wxButton* button4{ new wxButton(panel1, wxID_ANY, "Save") };
    wxButton* button5{ new wxButton(panel1, wxID_ANY, "Load") };

    wxGrid* grid = { new wxGrid(
                                    panel2,
                                    -1,
                                    wxPoint(-1,-1),
                                    wxSize(-1,-1),
                                    wxEXPAND
                                )
                    };

    void setControlslayout() override;

    void getGridData() const;
    

};


struct info
{
    int default_value;
    int max;
    int min;
    wxString default_value_string;
    wxString title;
};


class CSliderDialog final : public wxDialog
{
private:
    double threshold_value = 50.0;
protected:
    wxButton* m_button5;
    wxSlider* m_slider5;
    wxStaticText* m_staticText3;

public:

    CSliderDialog(  wxWindow* parent, 
                    info& inf,
                    wxWindowID id = wxID_ANY, 
                    const wxString& title = wxEmptyString, 
                    const wxPoint& pos = wxDefaultPosition, 
                    const wxSize& size = wxSize(301, 77), 
                    long style = wxDEFAULT_DIALOG_STYLE
                );

    ~CSliderDialog();

    double getValue() { return threshold_value; };

    Mat out;

};

class CInputDialog final : public wxDialog
{
private:

protected:
    wxButton* button1 = nullptr;
    wxButton* button2 = nullptr;
    wxComboBox* comboBox1 = nullptr;

    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
    std::string  setPath(bool Gray);
    void  setFinalImg(Mat& out);

    wxString SelectionText;

    void ApplyAlgorithm(Function1Parameter& f, bool Gray);
    void ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size);
    void ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2);
    void ApplyAlgorithm(Function4Parameters& f, bool Gray, int kernel_size, double p1, double p2);
    void ApplyAlgorithm(Function5Parameters& f, bool Gray, int kernel_size, int p1, int p2, int p3);
    void ApplyAlgorithm(FunctionSobelParameters& f, bool Gray, int, int, int, double, int);
    void ApplyAlgorithm(Function2Slider& f, bool Gray, double t);

    void fillComboInfo();

    Function1ParContainer fsimple;
    Function2ParContainer fadjust;
    Function2ParContainer fmore;
    Function3ParContainer fmore3;
    Function4ParContainer fmorep;
    Function5ParContainer fmorepp;
    Function2SliderContainer fslider;
    FunctionSobelParametersContainer fsobel;

    bool stop = false;

    wxString getSelectionText() { return SelectionText; };
    void DoFunction();

    std::function<Mat(Mat)>
        getAlgoFunctionOnePar(wxString key);

    std::function<Mat(Mat, int)>
        getAlgoFunctionTwoPar(wxString key);

    std::function<Mat(Mat, int, double, double)>
        getAlgoFunctionFourPar(wxString key);

    std::function<Mat(Mat, int, int, int, int)>
        getAlgoFunctionFivePar(wxString key);

    std::function<Mat(Mat, int, int)>
        getAlgoFunctionThreePar(wxString key);

    std::function<Mat(Mat, int)>
        getAlgoFunctionAdjust(wxString key);

    std::function<Mat(Mat, double)>
        getAlgoFunctionSlider(wxString key);

    FunctionSobelParameters
        getAlgoSobel(wxString key);

public:

    CInputDialog(   wxWindow* parent, 
                    CImageHelper* imghelper,
                    CWriteLogs* outxt,
                    wxWindowID id = wxID_ANY, 
                    const wxString& title = wxEmptyString, 
                    const wxPoint& pos = wxDefaultPosition, 
                    const wxSize& size = wxSize(335, 75), 
                    long style = wxDEFAULT_DIALOG_STYLE);

    ~CInputDialog();

};

#endif