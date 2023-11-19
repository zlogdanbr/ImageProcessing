#ifndef _CHILD_FRAME_DEFS_
#define _CHILD_FRAME_DEFS_


#include "image_helper.h"
#include <wx/grid.h>
#include "opcvwrapper.h"
#include "detection.h"
#include "logs.h"
#include <functional>


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

    //--------------------------------------------------------------
    // Components
    //--------------------------------------------------------------
    wxPanel* basePanel = new wxPanel(this, -1);
    wxPanel* panel1{ new wxPanel(basePanel) };
    wxPanel* panel2{ new wxPanel(basePanel, -1) };

    wxBoxSizer* baseSizer{ new wxBoxSizer(wxVERTICAL) };
    wxBoxSizer* hbox1{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* hbox2{ new wxBoxSizer(wxHORIZONTAL) };


    template<typename F>
    void
        ApplyAlgorithm(F& f, bool Gray);

    template<typename F>
    void
        ApplyAlgorithm(F& f, bool Gray, int kernel_size);



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
    //-------------------------------------------------------------
    wxButton* button1{ new wxButton(panel1, wxID_ANY, "OK")};
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel")};
    wxButton* button3{ new wxButton(panel1, wxID_ANY, "Clear")};
    wxButton* button4{ new wxButton(panel1, wxID_ANY, "Save") };
    wxButton* button5{ new wxButton(panel1, wxID_ANY, "Load") };

    // https://docs.wxwidgets.org/3.0/overview_grid.html
    wxGrid* grid = { new wxGrid(
                                    panel2,
                                    -1,
                                    wxPoint(-1,-1),
                                    wxSize(-1,-1),
                                    wxEXPAND
                                )
                    };
    
    // https://truelogic.org/wordpress/2021/12/17/5b-1-wxwidgets-wxboxsizer/
    void setControlslayout()
    {
        // set base sizer
        basePanel->SetSizer(baseSizer);

        // add buttons to the horizontal box
        hbox1->Add(button1);
        hbox1->Add(button2);
        hbox1->Add(button3);
        hbox1->Add(button4);
        hbox1->Add(button5);

        // add buttons to the horizontal box
        hbox2->Add(grid);

        // set horizontal base sizer at panel1 and panel2
        panel1->SetSizer(hbox1);
        panel2->SetSizer(hbox2);

        // add panel1 to the base sizer at the base panel
        baseSizer->Add(panel1);
        baseSizer->Add(panel2);

        grid->CreateGrid(13, 13);

        Center();
    }

};


class CInputDialog : public CInputDialogBase
{
public:

    CInputDialog(wxFrame* parent);

    std::function<Mat(Mat)> getAlgoFunctionSimple(wxString key)
    {
        if (fsimple.find(key) != fsimple.end())
        {
            return fsimple[key];
        }
        return nullptr;
    }

    std::function<Mat(Mat,int)> getAlgoFunctionMore(wxString key)
    {
        if (fmore.find(key) != fmore.end())
        {
            return fmore[key];
        }
        return nullptr;
    }

    wxString getSelectionText() { return SelectionText;};

    void DoFunction()
    {
        wxString opt = getSelectionText();

        std::function<Mat(Mat)> f1 = getAlgoFunctionSimple(opt);

        if (f1 == nullptr)
        {
            std::function<Mat(Mat, int)> f2 = getAlgoFunctionMore(opt);
            if (f2 == nullptr)
            {
                outxt->writeTo("Error while loading algos.\n");
                return;
            }

            ApplyAlgorithm(f2, false, 5);
        }
        else
        {
            ApplyAlgorithm(f1, false);
        }
    }


private:
    wxString SelectionText;
    wxComboBox* comboBox1{ new wxComboBox(panel1, wxID_ANY, wxEmptyString, { 10, 10 } )};
    wxButton* button1{ new wxButton(panel1, wxID_ANY, "Select") };
    wxButton* button2{ new wxButton(panel1, wxID_ANY, "Cancel") };

    void fillFSimple()
    {
        fsimple["Convert to Gray Scale"] = convertograyScale;
        fsimple["Equalize Gray Scale Image"] = equalizeGrayImage;
        fsimple["Equalize Color Scale Image"] = equalizeColorImage;
        fsimple["Apply Laplacian"] = laplacian;
        fsimple["Detect Corners"] = detectCorners;
        fsimple["Detect features"] = detect;
        fsimple["Custom Algo"] = custom_algo;
        fsimple["Detect Corners"] = detectCorners;
        fsimple["Detect features"] = detect;
        fsimple["Custom Algo"] = custom_algo;

        fsimple["Flip Image Horizontally"] = flipImageHorizontal;
        fsimple["Flip Image Vertically"] = flipImageVertical;
        fsimple["Flip Image"] = flipImage;

        comboBox1->Append("Convert to Gray Scale");
        comboBox1->Append("Equalize Gray Scale Image");
        comboBox1->Append("Equalize Color Scale Image");
        comboBox1->Append("Apply Laplacian");

        comboBox1->Append("Blur Image");
        comboBox1->Append("Gaussian");
        comboBox1->Append("Median");
        comboBox1->Append("Detect Corners");
        comboBox1->Append("Detect features");
        comboBox1->Append("Custom Algo");
        comboBox1->Append("Apply Threshold");
        comboBox1->Append("Flip Image Horizontally");
        comboBox1->Append("Flip Image Vertically");
        comboBox1->Append("Flip Image");

    }

    void fillFMore()
    {
        fmore["Blur Image"] = blurImageSmooth;
        fmore["Gaussian"] = GaussianImageSmooth;
        fmore["Median"] = MedianImageSmooth;
        fmore["Apply Threshold"] = ApplyThreShold;
    }

    void setControlslayout()
    {
        // set base sizer
        basePanel->SetSizer(baseSizer);

        // add buttons to the horizontal box
        hbox1->Add(button1);
        hbox1->Add(button2);

        // add buttons to the horizontal box
        hbox1->Add(comboBox1);

        // set horizontal base sizer at panel1 and panel2
        panel1->SetSizer(hbox1);

        // add panel1 to the base sizer at the base panel
        baseSizer->Add(panel1);

        fillFSimple();
        fillFMore();


        Center();
    }

    std::map < wxString, std::function<Mat(Mat)>> fsimple;
    std::map < wxString, std::function<Mat(Mat, int)>> fmore;


};

#endif