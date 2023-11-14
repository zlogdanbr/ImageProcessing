# Image Processing and WxWidgets

## Introduction

This project is my attempt to blend OpenCV and WxWidgets to re-learn Image Processing and at the same time, get more proficient on both libs.

There is another project I have here that shares the same code, but uses python, as a sort of make,  to build C++ sources: 

- https://github.com/zlogdanbr/images

## Tips

### Adding new algorithms

If you would like to add a new algorithm to the menu do the following

1- Create the ID using:
inline static const int ALGO_NODE_REC = 1;

2- Define the event handler
void OnNoduleRec(wxCommandEvent& event);

Inside this method you may either call
 ApplyAlgorithm(F& f, bool Gray);
 ApplyAlgorithm(F& f, bool Gray, int kernel_size);

- set Gray to True if the final image is a gray scale image because the application will use OpenCV
functions to save. For reasons I am not aware, the OpenCV save functions were not working for colored
images
- use the second method if the algorithm you would like to implement uses a kernel
( see opcvwrapper.h )

3- Add the event to event table
 Bind(wxEVT_MENU, &MyFrame::OnNoduleRec, this, ALGO_NODE_REC);

4- Add the new menu item
 auto menuItemNode= menuAlgo->Append(ALGO_NODE_REC, "Find Contours\tCTRL+F", "Find Contours")


### Image format

I am working with jpgs images only now but I plan to add options for more formats. Actually you
can change that option only by changing these lines:

void MyFrame::OnOpen(wxCommandEvent& event)

   wxFileDialog openFileDialog(this,
       wxEmptyString,
       wxEmptyString,
       wxEmptyString,
       "Image files (*.jpg)|*.jpg|All Files (*.*)|*.*",
       wxFD_OPEN | wxFD_FILE_MUST_EXIST);
 and 

void MyFrame::OnSave(wxCommandEvent& event)

   if (ImageHelper.getOriginalImageInitiated() == true)
   {
       auto name_final = ImageHelper.getOriginalImage().GetName();
       auto path = ImageHelper.getOriginalImage().GetPath();
       auto tosave = path + "\\" + name_final + "_proc_" + ".jpg";

-------------------------------------------------------------------------------------------------
## References:

- https://www.imageprocessingplace.com
- https://www.codtronic.com/blog/windows/building-wxwidgets-applications-using-microsoft-visual-studio/
- https://zetcode.com/gui/wxwidgets/
- https://wiki.wxwidgets.org/Avoiding_Memory_Leaks
- https://github.com/gammasoft71/Examples_wxWidgets
- https://github.com/gammasoft71/Examples_wxWidgets/blob/master/src/MenusAndToolbars/MainMenu/MainMenu.cpp
- https://www.developpez.net/forums/d1491398/c-cpp/bibliotheques/wxwidgets/opencv-transformer-cv-mat-wximage/
- https://stackoverflow.com/questions/8177747/converting-images-between-opencv-and-wxwidgets

-------------------------------------------------------------------------------------------------