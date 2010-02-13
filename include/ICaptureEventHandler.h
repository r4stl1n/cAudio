// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef ICAPTUREEVENTHANDLER_H
#define ICAPTUREEVENTHANDLER_H

namespace cAudio{

	class ICaptureEventHandler
	{

	public:
		//! This function is called on capture device intialize
		virtual void onInit() = 0;
		//! This function is called on capture buffer update
		virtual void onUpdate() = 0;
		//! This function is called on capture device shutdown
		virtual void onRelease() = 0;
		//! This function is called on capture begin
		virtual void onBeginCapture() = 0;
		//! This function is called on capture end
		virtual void onEndCapture() = 0;
		//! This function is called whe user request capture buffer
		virtual void onUserRequestBuffer() = 0;

	};

};

#endif //! ICAPTUREEVENTHANDLER_H
