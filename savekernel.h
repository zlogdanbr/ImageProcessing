//--------------------------------------------------------------------------------------------------
// used to save data that is entered in the wxGrid* grid dialog, eg, kernel definitions
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------
#ifndef _SAVEKERNEL_THIS_
#define _SAVEKERNEL_THIS_

#include "filesys.h"
#include <wx/grid.h>
#include <fstream>


bool SaveDataToFile(std::string, wxGrid* grid);
bool LoadDataFromFile(std::vector<std::vector<double>> obs, wxGrid* grid);
int readCSV(std::vector<std::vector<double>>& obs, std::string filename);
std::vector<double>& getcol(const std::vector<std::vector<double>>& obs, std::vector<double>& vec, int col);
std::string writeLine(std::vector<double>);

#endif