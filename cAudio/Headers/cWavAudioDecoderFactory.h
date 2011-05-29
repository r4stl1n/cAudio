// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include "../include/IAudioDecoderFactory.h"
#include "cWavDecoder.h"
#include "../Headers/cMutex.h"

#ifdef CAUDIO_COMPILE_WITH_WAV_DECODER

namespace cAudio
{
	class cWavAudioDecoderFactory : public IAudioDecoderFactory
	{
		public:
			cWavAudioDecoderFactory() {}
			~cWavAudioDecoderFactory() {}

			IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
			{      
				return CAUDIO_NEW cWavDecoder(stream);
			}
	};
};

#endif
