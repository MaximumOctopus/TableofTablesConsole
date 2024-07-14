//
// Table of Tables Console 2
//
// (c) Paul Alan Freshney 2016-2024
//   paul@freshney.org
//
// Inspired/based on the Behind the Tables sub-reddit.
//   https://www.reddit.com/r/BehindTheTables/
//
// Source code and executables:
//   https://github.com/MaximumOctopus/TableofTablesConsole
//

#include <ctime>
#include <string>

#include "Utility.h"


namespace Utility
{
    std::wstring ExtractFileName(const std::wstring& input)
    {
        size_t found;

        found = input.find_last_of(L"/\\");

        return input.substr(found + 1);
    }


    std::wstring ExtractFolder(const std::wstring& input)
    {
        size_t found;

        found = input.find_last_of(L"\\\\");

        return input.substr(0, found);
    }


    std::wstring ExtractFileNameNoExt(const std::wstring& input)
    {
        size_t found;

        found = input.find_last_of(L".");

        std::wstring noext = input.substr(0, found);

        found = input.find_last_of(L"/\\");

        if (found != std::string::npos)
        {
            noext = noext.substr(found + 1);
        }

        return noext;
    }


    bool Replace(std::wstring& str, const std::wstring& from, const std::wstring& to) 
    {
        size_t start_pos = str.find(from);

        if (start_pos == std::string::npos)
        {
            return false;
        }

        str.replace(start_pos, from.length(), to);

        return true;
    }


    bool ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to) // to do
    {
        size_t start_pos = str.find(from);

        while (start_pos != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);

            start_pos = str.find(from);
        }

        return true;
    }


    std::wstring PadRight(const std::wstring input, int pad)
    {
        if (input.length() < pad)
        {
            std::wstring output = input;

            for (int i = input.length(); i < pad; i++)
            {
                output = L" " + output;
            }

            return input;
        }

        return input;
    }


    std::wstring PadRight(int input, int pad)
    {
        std::wstring output = std::to_wstring(input);

        if (output.length() < pad)
        {
            for (int i = output.length(); i < pad; i++)
            {
                output = L" " + output;
            }

            return output;
        }

        return output;
    }


    std::wstring DateTime(int mode)
    {
        struct tm ltm;

        time_t now = time(0);

        localtime_s(&ltm, &now);

        std::wstring year = std::to_wstring(ltm.tm_year + 1900);
        std::wstring month = std::to_wstring(ltm.tm_mon + 1);
        std::wstring day = std::to_wstring(ltm.tm_mday);
        std::wstring hour = std::to_wstring(ltm.tm_hour);
        std::wstring min = std::to_wstring(ltm.tm_min);
        std::wstring sec = std::to_wstring(ltm.tm_sec);

        if (ltm.tm_mon < 10)
        {
            month = L"0" + month;
        }

        if (ltm.tm_mday < 10)
        {
            month = L"0" + month;
        }

        if (ltm.tm_hour < 10)
        {
            hour = L"0" + hour;
        }

        if (ltm.tm_min < 10)
        {
            min = L"0" + min;
        }

        if (ltm.tm_sec < 10)
        {
            sec = L"0" + sec;
        }

        if (mode == 0)
        {
            return year + L"/" + month + L"/" + day + L" " + hour + L":" + min + L"." + sec;
        }
        else
        {
            return year + month + day + hour + min + sec;
        }
    }
}