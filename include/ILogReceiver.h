// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef ILOGRECEIVER_H_INCLUDED
#define ILOGRECEIVER_H_INCLUDED

namespace cAudio
{
	enum LogLevel
	{
		ELL_DEBUG,
		ELL_INFO,
		ELL_WARNING,
		ELL_ERROR,
		ELL_CRITICAL,
		ELL_COUNT
	};

	const char* const LogLevelStrings[] =
	{
		"Debug",
		"Information",
		"Warning",
		"Error",
		"Critical",
		0
	};

	class ILogReceiver
	{
	public:
		ILogReceiver() { }
		~ILogReceiver() { }

		virtual bool OnLogMessage(const char* sender, const char* message, LogLevel level, float time) = 0;
	};
};
#endif //! ILOGRECEIVER_H_INCLUDED
