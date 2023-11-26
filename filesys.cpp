#include "filesys.h"
namespace fs = std::filesystem;

wxString getCurrentDir()
{
	wxString s = wxGetCwd();
	return s;
}

std::string getExtension(const std::string& _file)
{
	auto ext  = fs::path(_file).extension();
	return ext.string();
}

std::string getFileName(const std::string& _file)
{
	auto fname = fs::path(_file).filename();
	return fname.string();
}

std::string getPathOfFile(const std::string& _file)
{
	auto fpath = fs::path(_file).parent_path();
	return fpath.string();
}


bool file_exists(const std::string& file)
{
	return fs::exists(file);
}

uintmax_t getFileSize(const std::string& _file)
{
	std::filesystem::directory_entry File{ _file };
	return File.file_size();
}
// you can add try catches to every call, this is only an example
bool directory_exists(const std::string& _directory)
{
	try
	{
		std::filesystem::directory_entry Directory{ _directory };
		return Directory.exists();
	}
	catch (std::filesystem::filesystem_error e)
	{
		return false;
	}
}

bool is_directory(const std::string& _directory)
{
	std::filesystem::directory_entry Directory{ _directory };
	return Directory.is_directory();
}

bool is_file(const std::string& _directory)
{
	std::filesystem::directory_entry Directory{ _directory };
	return Directory.is_regular_file();
}

bool create_dir(const std::string& _directory)
{
	return std::filesystem::create_directory(_directory);
}
