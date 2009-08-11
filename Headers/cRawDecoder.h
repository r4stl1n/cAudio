#ifndef CRAWDECODER_H_INCLUDED
#define CRAWDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"

namespace cAudio
{

    class cRawDecoder : public IAudioDecoder
    {
        public:

			cRawDecoder(IDataSource* stream, unsigned int frequency, AudioFormats format);
            ~cRawDecoder();

            //!Retruns the format of the audio data
            virtual AudioFormats getFormat();

            //!Returns the frequency of the audio data
            virtual int getFrequency();

            //!Returns whether seeking is supported
            virtual bool isSeekingSupported();

            //!Reads a section of data out of the audio stream
            virtual int readAudioData(void* output, int amount);

            //!Sets the position to read data out of
            virtual bool setPosition(int position, bool relative);

            //!If seeking is supported, will seek the stream to seconds
            virtual bool seek(float seconds,bool relative);

        private:
            unsigned int Frequency;
			AudioFormats Format;
    };

}

#endif //! CRAWDECODER_H_INCLUDED