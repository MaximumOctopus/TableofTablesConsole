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

#pragma once


#include <string>


namespace Utility
{
    std::wstring ExtractFileName(const std::wstring&);
    std::wstring ExtractFileNameNoExt(const std::wstring&);

    std::wstring ExtractFolder(const std::wstring&);

    bool Replace(std::wstring&, const std::wstring&, const std::wstring&);
    bool ReplaceAll(std::wstring&, const std::wstring&, const std::wstring&);

    std::wstring PadRight(const std::wstring, int);
    std::wstring PadRight(int, int);

    std::wstring DateTime(int);
}