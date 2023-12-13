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
#include <functional>
#include <thread>
#include "image_helper.h"
#include "opcvwrapper.h"
#include "logs.h"
#include "filesys.h"

using Function1Parameter    = std::function<Mat(Mat)>;
using Function2Parameter    = std::function<Mat(Mat, int)>;
using Function3Parameters   = std::function<Mat(Mat, int, int)>;
using Function4Parameters   = std::function<Mat(Mat, int, double, double)>;
using Function5Parameters   = std::function<Mat(Mat, int, int, int, int)>;

using Function1ParContainer = std::map < wxString, Function1Parameter >;
using Function2ParContainer = std::map < wxString, Function2Parameter >;
using Function3ParContainer = std::map < wxString, Function3Parameters >;
using Function4ParContainer = std::map < wxString, Function4Parameters >;
using Function5ParContainer = std::map < wxString, Function5Parameters >;

using CPointCst = std::pair<int, int>;
using CDataValue = std::vector<CPointCst>;
using RGB_CST = unsigned char[3];

class COpenImage
{
public:

    COpenImage(CImageHelper* imghelper, CWriteLogs* outxt);

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };
    void OpenFile(wxFrame& frame);

private:

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
};

class CSaveImage
{
public:

    CSaveImage(CImageHelper* imghelper, CWriteLogs* outxt);

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };
    void SaveFile(wxFrame& frame);

private:

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
};

class CInputDialogBase : public wxFrame
{
public:

    CInputDialogBase(wxFrame* parent, wxString name);

    void setImageHelper(CImageHelper* imghlp) { imghelper = imghlp; };
    void setLogs(CWriteLogs* l) { outxt = l; };

protected:

    //--------------------------------------------------------------
    // Helpers
    //--------------------------------------------------------------
    CImageHelper* imghelper{ nullptr };
    CWriteLogs* outxt{ nullptr };
    std::string  setPath(bool Gray);
    void  setFinalImg(Mat& out);

    void ApplyAlgorithm(Function1Parameter& f, bool Gray);
    void ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size);
    void ApplyAlgorithm(Function3Parameters& f, bool Gray, int p1, int p2);
    void ApplyAlgorithm(Function4Parameters& f, bool Gray, int kernel_size, double p1, double p2);
    void ApplyAlgorithm(Function5Parameters& f, bool Gray, int kernel_size, int p1, int p2, int p3);

    // https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
    virtual void setControlslayout() = 0;

};

class CDataCapture : public CDataValue
{
public:
    void insertPoint(int x, int y)
    {
        CPointCst p{ x,y };
        this->push_back(p);
    }

    void clear_me()
    {
        this->clear();
    }

    bool isPointInThePicture(int x, int y)
    {
        CPointCst p(x, y);
        if (std::find(this->begin(), this->end(), p) != this->end())
        {
            return true;
        }
        return false;
    }
};

class CGridInputDialog : public CInputDialogBase
{
public:

    CGridInputDialog(wxFrame* parent);
    void getGridData() const;

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
    

};

class CInputDialog : public CInputDialogBase
{
public:

    CInputDialog(wxFrame* parent);

    std::function<Mat(Mat)> 
        getAlgoFunctionOnePar(wxString key);

    std::function<Mat(Mat, int)> 
        getAlgoFunctionTwoPar(wxString key);

    std::function<Mat(Mat, int, double, double)> 
        getAlgoFunctionFourPar(wxString key);

    std::function<Mat(Mat, int, int,int,int)>
        getAlgoFunctionFivePar(wxString key);

    std::function<Mat(Mat, int, int)>
        getAlgoFunctionThreePar(wxString key);

    std::function<Mat(Mat, int)>
        getAlgoFunctionAdjust(wxString key);

    wxString getSelectionText() { return SelectionText;};

    void DoFunction();


private:
    wxString SelectionText;
    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1{ new wxPanel(basePanel) };

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* hbox1{ new wxBoxSizer(wxHORIZONTAL) };

    wxComboBox* comboBox1{ new wxComboBox(panel1, wxID_ANY, wxEmptyString, { 10, 10 } )};
    wxButton* button1{ new wxButton(panel1, wxID_ANY, "Select") };
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel") };

    void fillComboInfo();
    void setControlslayout() override;

    Function1ParContainer fsimple;
    Function2ParContainer fadjust;
    Function2ParContainer fmore;
    Function3ParContainer fmore3;
    Function4ParContainer fmorep;
    Function5ParContainer fmorepp;

    bool stop = false;


};


class CSliderDialog : public CInputDialogBase
{
public:
    CSliderDialog(wxFrame* parent);

private:
    void setControlslayout() override;

    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1 = new wxPanel(basePanel, wxID_ANY, { -1,-1 }, { 400, 30 }, wxTAB_TRAVERSAL | wxBORDER_SIMPLE);
    wxPanel* panel2 = new wxPanel(basePanel, wxID_ANY, { -1,-1}, { 400, 30 }, wxTAB_TRAVERSAL | wxBORDER_THEME);

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxVERTICAL) };
    wxBoxSizer* hbox1{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* hbox2{ new wxBoxSizer(wxHORIZONTAL) };

    wxButton* button1{ new wxButton(panel1, wxID_ANY, "Select") };
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Run") };


    wxTextCtrl* textCtrl1 =     new wxTextCtrl(         panel2, 
                                                        wxID_ANY, 
                                                        "Script path", 
                                                        { -1,-1 },
                                                        { 380, 20 }); 
};

#endif