#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

bool directory_exists(const std::string& _directory);
bool file_exists(const std::string& );
bool is_directory(const std::string& _directory);
bool is_file(const std::string& _directory);
bool create_dir(const std::string& _directory);
uintmax_t getFileSize(const std::string& _file);
std::string getLastTime(const std::string& _file);
