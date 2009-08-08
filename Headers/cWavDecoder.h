#ifndef CWAVDECODER_H_INCLUDED
#define CWAVDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"

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

            //!Reads a section of data out of the audio stream
            virtual int readAudioData(void* output, int amount);

            //!Sets the position to read data out of
            virtual bool setPosition(int position, bool relative);

            //!If seeking is supported, will seek the stream to seconds
            virtual bool seek(float seconds,bool relative);

        private:
            int mChunkSize;
            int mSubChunk1Size;
            short mFormat;
            short mChannels;
            int mSampleRate;
            int mByteRate;
            short mBlockAlign;
            short mBitsPerSample;
            int mDataSize;
    };

}

#endif //! CFLACDECODER_H_INCLUDED

