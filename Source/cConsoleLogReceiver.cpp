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


