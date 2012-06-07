// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include "cAudioDefines.h"
#include "cAudioPlatform.h"
#include "cAudioMemory.h"
#include "cSTLAllocator.h"

#include <string>

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

#if CAUDIO_REROUTE_STRING_ALLOCATIONS == 1
	typedef std::basic_string< cAudioChar, std::char_traits<cAudioChar>, cSTLAllocator<cAudioChar> > cAudioString;
#else
#	if defined(UNICODE) || defined(_UNICODE)
		typedef std::basic_string<cAudioChar> cAudioString;
#	else
	typedef std::string cAudioString;
#	endif
#endif

    
#if defined(CAUDIO_PLATFORM_WIN) && (defined(UNICODE) || defined(_UNICODE))
    const TCHAR* toWINSTR(const char* str);
    
    const TCHAR* toWINSTR(const wchar_t* str);
    
    const char* toUTF8(const cAudioString& str);
    
    cAudioString fromUTF8(const char* str);    
#else
    inline const char* toWINSTR(const char* str) 
    {
        return str;
    }
    
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
