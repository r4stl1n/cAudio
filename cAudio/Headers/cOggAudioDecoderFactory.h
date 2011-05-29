// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include "../include/IAudioDecoderFactory.h"
#include "cOggDecoder.h"
#include "../Headers/cMutex.h"

#ifdef CAUDIO_COMPILE_WITH_OGG_DECODER

namespace cAudio
{
	class cOggAudioDecoderFactory : public IAudioDecoderFactory
	{
		public:
			cOggAudioDecoderFactory() {}
			~cOggAudioDecoderFactory() {}

			IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
			{
				return CAUDIO_NEW cOggDecoder(stream);
			}
	};
};

#endif
