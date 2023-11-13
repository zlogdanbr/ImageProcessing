#pragma once
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
#include  "segmentation.h"
#include "Eigen"
#include "Dense"
#include <opencv2/core/eigen.hpp>

bool convertOpenCVMathToEingein(const Mat& OpenCVImage);
