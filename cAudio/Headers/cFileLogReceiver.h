// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include "../include/ILogReceiver.h"
#include "../include/cAudioDefines.h"

#ifdef CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER
namespace cAudio
{
	
	class cFileLogReceiver : public ILogReceiver
	{
	public:
		cFileLogReceiver();
		~cFileLogReceiver();

		bool OnLogMessage(const char* sender, const char* message, LogLevel level, float time);

	private:

		bool firsttime;

	};
};
#endif