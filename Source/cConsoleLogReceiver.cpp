// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "../Headers/cConsoleLogReceiver.h"
#include <iostream>

namespace cAudio
{
    bool cConsoleLogReceiver::OnLogMessage(const char* sender, const char* message, LogLevel level, float time)
	{
		//std::cout << time << " " << sender << ": [" << LogLevelStrings[level] << "] " << message << std::endl;
		std::cout << "[" << LogLevelStrings[level] << "] " << message << std::endl;
		return true;
	}
};


