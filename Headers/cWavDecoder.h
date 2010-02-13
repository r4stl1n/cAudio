// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CWAVDECODER_H_INCLUDED
#define CWAVDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"
#include "../include/cAudioDefines.h"

#ifdef CAUDIO_COMPILE_WITH_WAV_DECODER

namespace cAudio
{

    class cWavDecoder : public IAudioDecoder
    {
        public:

            cWavDecoder(IDataSource* stream);
            ~cWavDecoder();

            //!Retruns the format of the audio data
            virtual AudioFormats getFormat();

            //!Returns the frequency of the audio data
            virtual int getFrequency();

            //!Returns whether seeking is supported
            virtual bool isSeekingSupported();

			//!Returns whether the stream is valid for this codec
			virtual bool isValid();

            //!Reads a section of data out of the audio stream
            virtual int readAudioData(void* output, int amount);

            //!Sets the position to read data out of
            virtual bool setPosition(int position, bool relative);

            //!If seeking is supported, will seek the stream to seconds
            virtual bool seek(float seconds,bool relative);

        private:
            short Channels;
            int SampleRate;
            int ByteRate;
            short BlockAlign;
            short BitsPerSample;
            int DataSize;
			int DataOffset;

			bool Valid;
    };

};

#endif

#endif //! CFLACDECODER_H_INCLUDED

