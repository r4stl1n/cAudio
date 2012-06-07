// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cAudioString.h"

#ifdef CAUDIO_PLATFORM_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <direct.h>
#  include <io.h>
#endif

#if defined(CAUDIO_PLATFORM_WIN) && (defined(UNICODE) || defined(_UNICODE))
    const TCHAR* toWINSTR(const char* str)
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
    
    const TCHAR* toWINSTR(const wchar_t* str)
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
    
    const char* toUTF8(const cAudioString& str)
    {
        static int id = 0;
        static char buffer[8][1024];
        id = ++id & 0x7;
        
        int buff_size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)(str.size() < 1023 ? str.size() : 1023), buffer[id], 1023, 0, false);
        buffer[id][buff_size] = 0;
        buffer[id][1023] = 0;
        return buffer[id];
    }
    
    cAudioString fromUTF8(const char* str)
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
#endif