//--------------------------------------------------------------------------------------------------
// Wrapper functions over c++17 filesystem libray
// author: Daniel Vasconcelos Gomes 2023
// if an external code has been used I indicate the sources
//--------------------------------------------------------------------------------------------------

#ifndef _MYFILESYS_FUNC_
#define _MYFILESYS_FUNC_

#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "wx/filefn.h"

bool directory_exists(const std::string& _directory);
bool file_exists(const std::string& );
bool is_directory(const std::string& _directory);
bool is_file(const std::string& _directory);
bool create_dir(const std::string& _directory);
uintmax_t getFileSize(const std::string& _file);
std::string getExtension(const std::string& _file);
std::string getFileName(const std::string& _file);
std::string getPathOfFile(const std::string& _file);
wxString getCurrentDir();


#endif
//--------------------------------------------------------------------------------------------------
