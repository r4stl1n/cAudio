// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IAUDIODECODER_H
#define IAUDIODECODER_H

#include "IRefCounted.h"
#include "IDataSource.h"
#include "EAudioFormats.h"

namespace cAudio
{
	class IAudioDecoder : public IRefCounted
	{
		public:
			IAudioDecoder(IDataSource* stream) : Stream(stream) { if(Stream) Stream->grab(); }
			virtual ~IAudioDecoder() { if(Stream) Stream->drop(); }

			//!Returns the format of the audio data
			virtual AudioFormats getFormat() = 0;

			//!Returns the frequency of the audio data
			virtual int getFrequency() = 0;

			//!Returns whether seeking is supported
			virtual bool isSeekingSupported() = 0;

			//!Returns whether the stream is valid for this codec
			virtual bool isValid() = 0;

			//!Reads a section of data out of the audio stream
			virtual int readAudioData(void* output, int amount) = 0;

			//!Sets the position to read data out of
			virtual bool setPosition(int position, bool relative) = 0;

			//!If seeking is supported, will seek the stream to seconds
			virtual bool seek(float seconds, bool relative) = 0;
		protected:
			IDataSource* Stream;
	};
};

#endif //! IAUDIODECODER_H
