// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CUTILS_H_INCLUDED
#define CUTILS_H_INCLUDED
#include "../include/cAudioPlatform.h"
#include <string>
#include <vector>

#ifdef CAUDIO_PLATFORM_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <direct.h>
#  include <io.h>
#endif

#ifdef CAUDIO_PLATFORM_LINUX
#	include <dirent.h> 
#	include <stdio.h> 
#	include <cstring>
#endif

//! Grabs the current extention of a given string.
static std::string getExt(const std::string& filename)
{
    if(filename.find_last_of(".") == std::string::npos) return filename;
    return filename.substr(filename.find_last_of(".") + 1, filename.length()-filename.find_last_of(".")-1);
}

//! Prevents a bug with NULL passed into std::string.
static std::string safeCStr(const char* str)
{
	if( str != NULL ) return std::string(str);
	else return std::string("");
}

//! Returns a list of files/directories in the supplied directory.  Used internally for auto-installation of plugins.
static std::vector<std::string> getFilesInDirectory(std::string path)
{
	std::vector<std::string> FileList;
#ifdef CAUDIO_PLATFORM_WIN
	std::string search = path + "\\" + std::string("*.*");
	WIN32_FIND_DATA info;
	HANDLE h = FindFirstFile(search.c_str(), &info);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(strcmp(info.cFileName, ".") == 0 || strcmp(info.cFileName, "..") == 0))
			{
				FileList.push_back(info.cFileName);
			}
		} while (FindNextFile(h, &info));
		FindClose(h);
	}
#endif

#ifdef CAUDIO_PLATFORM_LINUX
	DIR           *d;
	struct dirent *dir;
	d = opendir(path.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ) { continue; }
			if( dir->d_type == DT_DIR ) continue;
			FileList.push_back(dir->d_name);
		}

		closedir(d);
	}
#endif

	return FileList;
}

#endif //! CUTILS_H_INCLUDED
