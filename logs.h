#ifndef _MY_APP_LOGS_
#define _MY_APP_LOGS_

#include "image_helper.h"
#include "childframes.h"
#include <chrono>

class CWriteLogs
{
public:
    CWriteLogs(wxTextCtrl* ptrtextCtrl) :textCtrl{ ptrtextCtrl } {};
    virtual ~CWriteLogs() {};

    void writeTo(wxString& msg)
    {
        textCtrl->AppendText(return_current_time_and_date() + ": " + msg);
    }

    void writeTo(const char* msg)
    {
        textCtrl->AppendText(return_current_time_and_date() + ": " + msg);
    }

    void writeInfo(std::stringstream& os)
    {
        textCtrl->AppendText(os.str().c_str());
    }

    std::string return_current_time_and_date()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }


private:
    CWriteLogs(CWriteLogs&) = delete;
    CWriteLogs& operator=(CWriteLogs&) = delete;
    wxTextCtrl* textCtrl;


};

#endif