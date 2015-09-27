// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include "cAudioDefines.h"
#include "cAudioPlatform.h"
#include "cAudioMemory.h"
#include "cSTLAllocator.h"

#include <string>
#include <stdlib.h>

#ifdef CAUDIO_PLATFORM_WIN
#  include <direct.h>
#  include <io.h>
#endif

namespace cAudio
{

#if defined(UNICODE) || defined(_UNICODE)
#	define _CTEXT(x)	L ## x
#	define cstrcmp		wcscmp
#	define cAudioChar	wchar_t
#	define cfopen(N, M)	_wfopen((N).c_str(), L ## M)
#else
#	define _CTEXT(x) x
#	define cstrcmp		strcmp
#	define cAudioChar	char
#	define cfopen(N, M)	fopen(toUTF8(N), M)
#endif

#if CAUDIO_REROUTE_STRING_ALLOCATIONS == 1
	typedef std::basic_string< cAudioChar, std::char_traits<cAudioChar>, cSTLAllocator<cAudioChar> > cAudioString;
#else
#	if defined(UNICODE) || defined(_UNICODE)
		typedef std::basic_string<cAudioChar> cAudioString;
#	else
	typedef std::string cAudioString;
#	endif
#endif

    
#if defined(CAUDIO_PLATFORM_WIN)
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
        int str_len = (int)strlen(str);
        int buf_size = MultiByteToWideChar(CP_UTF8, 0, str, str_len, 0, 0);
        cAudioString s(buf_size, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, str, str_len, &s[0], buf_size);
        return s;
    }
    
#else
    inline const char* toUTF8(const cAudioString& str)
    {
        return str.c_str();
    }

    inline cAudioString fromUTF8(const char* str)
    {
        return cAudioString(str);
    }
#endif
};

