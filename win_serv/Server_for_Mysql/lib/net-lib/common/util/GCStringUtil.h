#pragma once

//多字节与宽字节字符UTF-8 编码转
//与GCHUtilCString.h功能一样
//不过这模块给重新封装过

#ifndef __StringUtil_H_
class GCStringUtil
{
public:
    static string ws2s(const wstring& ws)
    {
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);
        char *_Dest = new char[_Dsize];
        WideCharToMultiByte(CP_ACP, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
        string result = _Dest;
        delete []_Dest;
        return result;
    }
    static wstring s2ws(const string& s)
    {
        const char* _Source = s.c_str();
        size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
        wchar_t *_Dest = new wchar_t[_Dsize];
        MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);
        wstring result = _Dest;
        delete []_Dest;
        return result;
    }
    static string ws2utf8(const wstring& ws)
    {
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = WideCharToMultiByte(CP_UTF8, 0, _Source, -1, NULL, 0, NULL, NULL);
        char *_Dest = new char[_Dsize];
        WideCharToMultiByte(CP_UTF8, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
        string result = _Dest;
        delete []_Dest;
        return result;
    }
    static wstring utf82ws(const string& s)
    {
        const char* _Source = s.c_str();
        size_t _Dsize = MultiByteToWideChar(CP_UTF8, 0, _Source, -1, NULL, 0);
        wchar_t *_Dest = new wchar_t[_Dsize];
        MultiByteToWideChar(CP_UTF8, 0, _Source, -1, _Dest, _Dsize);
        wstring result = _Dest;
        delete []_Dest;
        return result;
    }
};
#ifdef _UNICODE
 #define _WS2S(str)  GCStringUtil::ws2s(str).c_str()
 #define _S2WS(str)  GCStringUtil::s2ws(str).c_str()
#else
 #define _WS2S(str)  str
 #define _S2WS(str)  str
#endif
#define __StringUtil_H_
#endif