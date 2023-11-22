#include "childframes.h"
#include "savekernel.h"

void CInputDialog::DoFunction()
{
    wxString opt = getSelectionText();

    Function1Parameter f1 = getAlgoFunctionSimple(opt);

    if (f1 == nullptr)
    {
        Function2Parameter f2 = getAlgoFunctionMore(opt);
        if (f2 == nullptr)
        {
            outxt->writeTo("Error while loading algos.\n");
            return;
        }

        ApplyAlgorithm(f2, true, 5);
    }
    else
    {
        ApplyAlgorithm(f1, true);
    }
}

CInputDialog::CInputDialog(wxFrame* parent) :CInputDialogBase{ parent,"Basic Algorithms Selection" }
{
    setControlslayout();

    button1->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // set values
            int item = comboBox1->GetSelection();
            SelectionText = comboBox1->GetValue();
            DoFunction();
            Close();
        });

    button2->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event)
        {
            // cancel
            Close();
        });
}
