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
#include <wx/spinctrl.h>
#include <functional>
#include <thread>
#include "image_helper.h"
#include "opcvwrapper.h"
#include "logs.h"
#include "filesys.h"
#include "image_util.h"

using namespace image_util;

constexpr int INVALID_VALUE_INT = -90566;
constexpr double INVALID_VALUE_DOUBLE = -9056698.01;

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
    void ApplyAlgorithm(FunctionHarris& f, bool Gray, int, int, double, double);

    template<typename F, typename...Args>
    void ApplyAlgorithmEffective(F& f, bool Gray, Args&&... args);

    void fillComboInfo();

    Function1ParContainer fsimple;
    Function2ParContainer fadjust;
    Function2ParContainer fmore;
    Function3ParContainer fmore3;
    Function4ParContainer fmorep;
    Function5ParContainer fmorepp;
    Function2SliderContainer fslider;
    FunctionSobelParametersContainer fsobel;
    FunctionHarrisParametersContainer fharris;

    bool stop = false;

    wxString getSelectionText() { return SelectionText; };

    void setSimpleMaps();
    void setOtherMaps();

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

    FunctionHarris
        getAlgoHarris(wxString key);

    std::vector<wxString> 
        algo_names =
                            { 
                                "Undo",
                                "Convert to Gray Scale",
                                "Equalize Gray Scale Image",
                                "Equalize Color Scale Image",
                                "Flip Image Horizontally",
                                "Flip Image Vertically",
                                "Flip Image",
                                "Blur Image",
                                "Invert Image",
                                "Convert to Binary",
                                "Threshold",
                                "Adjust Contrast",
                                "Adjust Brightness",
                                "Gamma Correction",
                                "Erode",
                                "Dilate",
                                "Closing",
                                "Opening",
                                "Erosion+",
                                "Dilate+",
                                "Segmentation Erode",
                                "Morpholgical Gradient",
                                "Morphological Top Hat",
                                "Apply custom algo",
                                "Gaussian Extended",
                                "Difference of Gaussians",
                                "Median",
                                "Sharpening",
                                "Unsharp",
                                "Laplacian Extended",
                                "Sobel",
                                "Canny Extended",
                                "Hough Transform Custom",
                                "Harris Algorithm",
                                "Find Contourns",
                                "PCA"
                            };

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


class CNumentryDouble : public wxDialog
{
private:

protected:
    wxButton* m_button2;
    wxSpinCtrlDouble* m_spinCtrlDouble1;

public:

    wxSpinCtrlDouble* getComponentValue()
    {
        return m_spinCtrlDouble1;
    };

    CNumentryDouble(  wxWindow* parent, 
                double max,
                double min,
                double increment,
                wxWindowID id = wxID_ANY, 
                const wxString& title = wxEmptyString, 
                const wxPoint& pos = wxDefaultPosition, 
                const wxSize& size = wxSize(191, 79), 
                long style = wxDEFAULT_DIALOG_STYLE);

    ~CNumentryDouble();

};


class CLoadImageSet : public wxDialog
{
private:

protected:

    wxPanel* m_panel5;
    wxPanel* m_panel6;

    wxButton* m_buttonOK;
    wxButton* m_buttonClear;
    wxButton* m_buttonLoadImage1;
    wxButton* m_buttonLoadImage2;
    wxButton* m_buttonLoadImage3;
    wxButton* m_buttonLoadImage4;
    wxButton* m_buttonLoadImage5;
    wxButton* m_buttonLoadImage6;
    wxButton* m_buttonLoadImage7;
    wxButton* m_buttonLoadImage8;

    wxTextCtrl* m_textCtrlImagePath1;
    wxTextCtrl* m_textCtrlImagePath2;
    wxTextCtrl* m_textCtrlImagePath3;
    wxTextCtrl* m_textCtrlImagePath4;
    wxTextCtrl* m_textCtrlImagePath5;
    wxTextCtrl* m_textCtrlImagePath6;
    wxTextCtrl* m_textCtrlImagePath7;
    wxTextCtrl* m_textCtrlImagePath8;

    void setTextAtBox(wxTextCtrl* txtctrl, wxString text)
    {
        txtctrl->SetLabelText(text);
    }

    wxString getTextFromBox(wxTextCtrl* txtctrl)
    {
        return txtctrl->GetLabelText();
    }

    std::vector< wxTextCtrl* > _paths;
    std::vector< wxButton* > _actions;

    bool readImagePath( wxWindow* parent, wxTextCtrl* txtctrl);

    std::vector<Mat> _images;

    void setImageArray();

    void setEventButtons();

public:

    CLoadImageSet(  wxWindow* parent, 
                wxWindowID id = wxID_ANY, 
                const wxString& title = wxEmptyString, 
                const wxPoint& pos = wxDefaultPosition, 
                const wxSize& size = wxSize(482, 408),
                long style = wxDEFAULT_DIALOG_STYLE);

    ~CLoadImageSet();

    
    std::vector<Mat> getImages() const
    {
        return _images;
    }

    void doProcess();

    bool IsoK = false;
};

#endif