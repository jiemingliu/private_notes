/********************************************************************
	created:	2016/06/29
	created:	29:6:2016   11:03
	file base:	data_convert
	file ext:	hpp
	author:		Huangl
	
	purpose:	字符编码转换
*********************************************************************/
#pragma once

#include <windows.h>
#include <string>

namespace TGS {
namespace Convert {

/// <summary>
/// Author： Huangl
/// UTF8转换至ANSI
/// </summary>
/// <param name="utf8"></param>
/// <param name="ansi"></param>
/// <returns></returns>
inline void utf8ToAnsi(const std::string& utf8, std::string& ansi)
{
    int wLength = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
    wchar_t* wChar = new wchar_t[wLength * sizeof(wchar_t)];
    memset(wChar, 0, wLength * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wChar, wLength);

    int ansiLength = WideCharToMultiByte(CP_ACP, 0, wChar, wLength, 0, 0, 0, 0);
    ansi.resize(ansiLength);
    WideCharToMultiByte(CP_ACP, 0, wChar, wLength, (char*)ansi.c_str(), ansiLength, 0, 0);
    delete[] wChar;
}

inline std::string utf8ToAnsi(const std::string& utf8)
{
    std::string ansi;
    utf8ToAnsi(utf8, ansi);
    return ansi;
}
/// <summary>
/// Author： Huangl
/// ANSI转换至UTF8
/// </summary>
/// <param name="ansi"></param>
/// <param name="utf8"></param>
/// <returns></returns>
inline void ansiToUtf8(const std::string& ansi, std::string& utf8)
{
    int wLength = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), NULL, 0);
    wchar_t* wChar = new wchar_t[wLength * sizeof(wchar_t)];
    memset(wChar, 0, wLength * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, wChar, wLength);

    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wChar, wLength, 0, 0, 0, 0);
    utf8.resize(utf8Length);
    WideCharToMultiByte(CP_UTF8, 0, wChar, wLength, (char*)utf8.c_str(), utf8Length, 0, 0);
    delete[] wChar;
}

inline std::string ansiToUtf8(const std::string& ansi)
{
    std::string utf8;
    ansiToUtf8(ansi, utf8);
    return utf8;
}

inline void strToWStr(const std::string& ansi, std::wstring& ws, int srcType = CP_ACP)
{
    int len = MultiByteToWideChar(srcType, 0, ansi.c_str(), ansi.size(), NULL, 0);
    ws.resize(len);
    MultiByteToWideChar(srcType, 0, ansi.c_str(), -1, &ws.front(), len);
}

inline std::wstring strToWStr(const std::string& ansi, int srcType = CP_ACP)
{
    std::wstring ws;
    strToWStr(ansi, ws, srcType);
    return ws;
}

inline void wstrToStr(const std::wstring& wstr, std::string& str, int tarType = CP_ACP)
{
    int strLength = WideCharToMultiByte(tarType, 0, wstr.c_str(), wstr.size(), 0, 0, 0, 0);
    str.resize(strLength);
    WideCharToMultiByte(tarType, 0, wstr.c_str(), wstr.size(), (char*)str.c_str(), strLength, 0, 0);
}

inline std::string wstrToStr(const std::wstring& str, int tarType = CP_ACP)
{
    std::string wstr;
    wstrToStr(str, wstr, tarType);
    return wstr;
}

} // Convert
} // TGS
