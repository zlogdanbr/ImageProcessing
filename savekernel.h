#pragma once

#include "filesys.h"
#include <wx/grid.h>
#include <fstream>


bool SaveDataToFile(std::string, wxGrid* grid);
bool LoadDataToFile(std::vector<std::vector<double>> obs, wxGrid* grid);
int readCSV(std::vector<std::vector<double>>& obs, std::string filename);
std::vector<double>& getcol(const std::vector<std::vector<double>>& obs, std::vector<double>& vec, int col);
std::string writeLine(std::vector<double>);
