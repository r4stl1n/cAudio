#ifndef CAUDIOSTRING_H_INCLUDED
#define CAUDIOSTRING_H_INCLUDED

#include "cAudioDefines.h"
#include "cAudioPlatform.h"
#include "cAudioMemory.h"
#include "cSTLAllocator.h"

#include <string>

#ifdef CAUDIO_PLATFORM_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <direct.h>
#  include <io.h>
#endif

namespace cAudio
{

#if defined(UNICODE) || defined(_UNICODE)
#	define _CTEXT(x)	L ## x
#	define cstrcmp		wcscmp
#	define cAudioChar	wchar_t
#	define cfopen		_wfopen
#else
#	define _CTEXT(x) x
#	define cstrcmp		strcmp
#	define cAudioChar	char
#	define cfopen		fopen
#endif

#ifdef CAUDIO_REROUTE_STL_ALLOCATIONS
	typedef std::basic_string< cAudioChar, std::char_traits<cAudioChar>, cSTLAllocator<cAudioChar> > cAudioString;
#else
	//typedef std::string cAudioString;
	typedef std::basic_string<cAudioChar> cAudioString;
#endif

    
#if defined(CAUDIO_PLATFORM_WIN) && (defined(UNICODE) || defined(_UNICODE))
    static const TCHAR* toWINSTR(const char* str)
    {
        static int id = 0;
        static wchar_t buffer[8][1024];
        id = ++id & 0x7;
        
        int slen = strlen(str);
        int buff_size = MultiByteToWideChar(CP_UTF8, 0, str, (int)(slen < 1023 ? slen : 1023), buffer[id], 1023);
        buffer[id][buff_size] = 0;
        buffer[id][1023] = 0;
        return buffer[id];
    }
    
    static const TCHAR* toWINSTR(const wchar_t* str)
    {
        static int id = 0;
        static char buffer[8][1024];
        id = ++id & 0x7;
    
        int slen = wcslen(str);
        int buff_size = WideCharToMultiByte(CP_UTF8, 0, str, (int)(slen < 1023 ? slen : 1023), buffer[id], 1023, 0, false);
        buffer[id][buff_size] = 0;
        buffer[id][1023] = 0;
        return buffer[id];
    }
    
    static const char* toUTF8(const cAudioString& str)
    {
        static int id = 0;
        static char buffer[8][1024];
        id = ++id & 0x7;
        
        int buff_size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)(str.size() < 1023 ? str.size() : 1023), buffer[id], 1023, 0, false);
        buffer[id][buff_size] = 0;
        buffer[id][1023] = 0;
        return buffer[id];
    }
    
    static cAudioString fromUTF8(const char* str)
    {
        wchar_t* buffer = 0;
        int buff_size = MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), 0, 0);
        if (buff_size == 0)
            return cAudioString();
        
        
        buffer = new wchar_t[buff_size + 1];
        memset((void*)buffer, 0, sizeof(wchar_t) * (buff_size + 1));
        MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), buffer, buff_size);
        cAudioString s(buffer);
        delete[] buffer;
        return s;
    }
    
#else
    static const char* toWINSTR(const char* str) 
    {
        return str;
    }
    
    static const char* toUTF8(const cAudioString& str)
    {
        return str.c_str();
    }
        
    static cAudioString fromUTF8(const char* str)
    {
        return cAudioString(str);
    }
#endif
};

#endif //! CAUDIOSTRING_H_INCLUDED
