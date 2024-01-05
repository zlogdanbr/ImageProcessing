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
#include <wx/busyinfo.h>
#include <functional>
#include <thread>
#include <mutex>
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


class CGridDialogInput : public wxDialog
{
private:

protected:
    wxButton* button1;
    wxButton* button2;
    wxButton* button3;
    wxButton* button4;
    wxButton* button5;
    wxGrid* grid;

    void getGridData();

    Mat final;

public:

    CGridDialogInput(  wxWindow* parent,
                Mat& img,
                wxWindowID id = wxID_ANY, 
                const wxString& title = wxEmptyString, 
                const wxPoint& pos = wxDefaultPosition, 
                const wxSize& size = wxSize(1188, 333), 
                long style = wxDEFAULT_DIALOG_STYLE);

    ~CGridDialogInput();

    Mat getFinal() { return final; };

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

    Mat original;
    Mat final_image;
    ImageCache revertContainer;

    wxButton* button1 = nullptr;
    wxButton* button2 = nullptr;
    wxButton* button3 = nullptr;
    wxComboBox* comboBox1 = nullptr;
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

    void fillComboInfo();
    void setSimpleMaps();
    void setOtherMaps();

    wxBusyInfo* ProgramBusy()
    {
        std::mutex mtex;
        mtex.lock();
        wxWindowDisabler disableAll;
        wxBusyInfo* wait = new wxBusyInfo("Please wait, working...");
        mtex.unlock();
        return wait;
    }

    void Stop(wxBusyInfo* wait)
    {
        wxYield();
        if (nullptr != wait)
        {
            delete wait;
        }
    }

    void setOriginalImage()
    {
        std::mutex mtex;
        mtex.lock();
        original.deallocate();
        original = final_image.clone();
        revertContainer.AddImgToCache(original);
        mtex.unlock();
        shouldQuit = true;
    }

    void revert()
    {
        Mat TopOf;
        if (revertContainer.getFirstOfCache(TopOf) == true)
        {
            final_image.deallocate();
            final_image = TopOf;
            return;
        }
        else
        {
            if (original.empty() == false)
            {
                final_image.deallocate();
                final_image = actual_first;
                setOriginalImage();
            }
        }
    }

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

    bool shouldQuit = false;
    bool DoFunctionBasedOnNameAlgo(wxString& _algorithm);
    bool DoFunctionBasedOnFunctor(wxString& _algorithm);
    void DoFunction();
    Mat actual_first;

public:

    CInputDialog(   wxWindow* parent,
                    const Mat& original,
                    wxWindowID id = wxID_ANY,
                    const wxString& title = wxEmptyString, 
                    const wxPoint& pos = wxDefaultPosition, 
                    const wxSize& size = wxSize(467, 75),
                    long style = wxDEFAULT_DIALOG_STYLE);

    ~CInputDialog();

    Mat getOutPutImage() { return final_image; };

    bool op_cancelled = false;

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


class CLoadImageSetBase : public wxDialog
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

    std::vector< wxTextCtrl* > _paths = {   m_textCtrlImagePath1 ,
                                            m_textCtrlImagePath2 ,
                                            m_textCtrlImagePath3 ,
                                            m_textCtrlImagePath4 ,
                                            m_textCtrlImagePath5 ,
                                            m_textCtrlImagePath6 ,
                                            m_textCtrlImagePath7 ,
                                            m_textCtrlImagePath8 };

    std::vector< wxButton* > _actions = {   m_buttonLoadImage1 ,
                                            m_buttonLoadImage2 ,
                                            m_buttonLoadImage3 ,
                                            m_buttonLoadImage4 ,
                                            m_buttonLoadImage5 ,
                                            m_buttonLoadImage6 ,
                                            m_buttonLoadImage7 ,
                                            m_buttonLoadImage8 };


    bool readImagePath( wxWindow* parent, wxTextCtrl* txtctrl);

    std::vector<Mat> _images;
    std::vector<std::string> _filenames;

    void setImageArray();

    void setEventButtons();

    CWriteLogs* outxt = nullptr;

    int inputs = 2;

    void AddButtons(wxBoxSizer* sizer1);
    void AddTextCrlt(wxBoxSizer* sizer2);

public:

    CLoadImageSetBase(  wxWindow* parent,
                        CWriteLogs* outxt,
                        wxWindowID id = wxID_ANY, 
                        const wxString& title = wxEmptyString, 
                        int inputs = 2,
                        const wxPoint& pos = wxDefaultPosition, 
                        const wxSize& size = wxSize(482, 408),
                        long style = wxDEFAULT_DIALOG_STYLE);

    ~CLoadImageSetBase();

    
    std::vector<Mat> getImages() const
    {
        return _images;
    }

    virtual void doProcess() = 0;

    bool IsoK = false;

    
};

class CApplySift : public CLoadImageSetBase
{
public:
    CApplySift( wxWindow* parent,
                CWriteLogs* outxt,
                wxWindowID id = wxID_ANY,
                const wxString& title = wxEmptyString,
                int inputs = 2);

    virtual void doProcess() override;
};

class CMatchTemplate : public CLoadImageSetBase
{
public:
    CMatchTemplate(wxWindow* parent,
        CWriteLogs* outxt,
        wxWindowID id = wxID_ANY,
        const wxString& title = wxEmptyString,
        int inputs = 2);

    virtual void doProcess() override;
};


class CMatchTemplateFull : public CLoadImageSetBase
{
public:
    CMatchTemplateFull(wxWindow* parent,
        CWriteLogs* outxt,
        wxWindowID id = wxID_ANY,
        const wxString& title = wxEmptyString,
        int inputs = 8);

    virtual void doProcess() override;
};

struct template_info
{
    wxString mode = "TM_SQDIFF";
    wxString Algorithm = "Canny";
    double t1 = 125;
    double t2 = 350;
};

class CSelectTemplateParam : public wxDialog
{
private:

protected:
    wxButton* m_button5;
    wxButton* m_button6;
    wxStaticText* m_staticText5;
    wxStaticText* m_staticText6;
    wxStaticText* m_staticText7;
    wxComboBox* m_comboBox3;
    wxSpinCtrlDouble* m_spinCtrlDouble3;
    wxSpinCtrlDouble* m_spinCtrlDouble4;

public:

    CSelectTemplateParam(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(477, 302), long style = wxDEFAULT_DIALOG_STYLE);

    ~CSelectTemplateParam();

    template_info _inf;
    bool IsOK;

};



#endif