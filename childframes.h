#ifndef _CHILD_FRAME_DEFS_
#define _CHILD_FRAME_DEFS_


#include "image_helper.h"
#include <wx/grid.h>
#include "opcvwrapper.h"
#include "detection.h"
#include "logs.h"
#include <functional>
#include <wx/numdlg.h>
#include <thread>
#include <wx/wx.h>
#include <wx/progdlg.h>
#include "filesys.h"

using Function1Parameter = std::function<Mat(Mat)>;
using Function2Parameter = std::function<Mat(Mat, int)>;
using Function1ParContainer = std::map < wxString, Function1Parameter >;
using Function2ParContainer = std::map < wxString, Function2Parameter >;

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

    void ApplyAlgorithm(Function1Parameter& f, bool Gray);
    void ApplyAlgorithm(Function2Parameter& f, bool Gray, int kernel_size);

    // https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
    virtual void setControlslayout() = 0;

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
    void setControlslayout();

};


class CInputDialog : public CInputDialogBase
{
public:

    CInputDialog(wxFrame* parent);

    std::function<Mat(Mat)> getAlgoFunctionSimple(wxString key);
    std::function<Mat(Mat, int)> getAlgoFunctionMore(wxString key);

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
    void setControlslayout();

    Function1ParContainer fsimple;
    Function2ParContainer fmore;

    bool stop = false;


};


/**
* TODO: This will have some tools for editing images but xwWidgets methods will be used
* instead
*/
class CImageCustomDialog : public CInputDialogBase
{
public:

    CImageCustomDialog(wxFrame* parent);

    void loadImage();
    void SaveImage();
    void reloadImage(int factor1 = 710, int factor2 = 710);

    wxImage getImage() { return image; };

private:

    int w = 0;
    int h = 0;

    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1{ new wxPanel(basePanel) };
    wxPanel* panel2{ new wxPanel(basePanel, -1) };

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* vbox1{ new wxBoxSizer(wxVERTICAL) };
    wxBoxSizer* vbox2{ new wxBoxSizer(wxVERTICAL) };

    wxButton* button5{ new wxButton(panel1, wxID_ANY, "Load") };
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel") };
    wxButton* button3{ new wxButton(panel1, wxID_ANY, "Rescale-") };
    wxButton* button6{ new wxButton(panel1, wxID_ANY, "Rescale+") };
    wxButton* button4{ new wxButton(panel1, wxID_ANY, "Save") };        
    wxButton* button8{ new wxButton(panel1, wxID_ANY, "Rotate+") };
    wxButton* button9{ new wxButton(panel1, wxID_ANY, "Rotate-") };


    wxImage image;
    wxStaticBitmap* picture = new wxStaticBitmap(   panel2, wxID_ANY, wxNullBitmap, { -1,-1 }, {700,700}, wxBORDER_SUNKEN);

    std::vector<int> resize_factor;

    bool pog_mirror = false;

    void setControlslayout();
};

#endif