// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifdef _WIN32
#include <windows.h>	//Basic windows include for Sleep();
#else
#include <unistd.h>		//Assumed linux system, include for usleep()
#include <time.h>
#endif					//If you need to support another platform, simply add a define for it

#include "../include/cAudioSleep.h"

namespace cAudio
{

void cAudioSleep(unsigned int ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms*1000); //convert from milliseconds to microseconds
#endif
}

};