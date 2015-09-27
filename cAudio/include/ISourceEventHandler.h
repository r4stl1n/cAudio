// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

namespace cAudio
{
	//! Interface for event handlers on Audio Sources.
	class ISourceEventHandler
	{
	public:
		//! This function is called when a source updates its buffers.
		virtual void onUpdate(){}

		//! This function is called when a source is released and soon to be deleted.
		virtual void onRelease(){}

		//! This function is called when a source starts playing.
		virtual void onPlay(){}

		//! This function is called when a source stopped playback.
		virtual void onStop(){}

		//! This function is called when a source is paused.
		virtual void onPause(){}
        
        virtual ~ISourceEventHandler(){}
	};
};

