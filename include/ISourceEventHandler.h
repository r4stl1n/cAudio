// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef ISOURCEEVENTHANDLER_H
#define ISOURCEEVENTHANDLER_H

namespace cAudio{

	class ISourceEventHandler
	{

	public:
		//! This function calls on source initalization
		virtual void onInit() = 0;
		//! This function calls on source update
		virtual void onUpdate() = 0;
		//! This function calls on source release
		virtual void onRelease() = 0;
		//! This function calls on source play
		virtual void onPlay() = 0;
		//! This function calls on source stop
		virtual void onStop() = 0;
		//! This function calls on source pause
		virtual void onPause() = 0;

	};

};

#endif //! ISOURCEEVENTHANDLER_H
