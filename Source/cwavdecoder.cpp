#include "../Headers/cWavDecoder.h"

namespace cAudio{

    cWavDecoder::cWavDecoder(IDataSource* stream) : IAudioDecoder(stream)
    {
        Stream->seek(4,false);
        Stream->read(&mChunkSize,4);
        Stream->seek(16,false);
        Stream->read(&mSubChunk1Size,4);
        Stream->read(&mFormat,sizeof(short));
        Stream->read(&mChannels,sizeof(short));
        Stream->read(&mSampleRate,sizeof(int));
        Stream->read(&mByteRate,sizeof(int));
        Stream->read(&mBlockAlign,sizeof(short));
        Stream->read(&mBitsPerSample,sizeof(short));
        Stream->seek(40,false);
        Stream->read(&mDataSize,sizeof(int));
        Stream->seek(44,false);

		//Double the sampleRate to fix a bug with half-time speed
        mSampleRate += mSampleRate;
    }

    cWavDecoder::~cWavDecoder()
    {
        mChunkSize = 0;
        mSubChunk1Size = 0;
        mFormat = 0;
        mChannels = 0;
        mSampleRate = 0;
        mByteRate = 0;
        mBlockAlign = 0;
        mBitsPerSample = 0;
        mDataSize = 0;
    }

    //!Returns wav channel format
    AudioFormats cWavDecoder::getFormat()
    {
        if(mChannels = 1)
            return EAF_16BIT_MONO;
        else
            return EAF_16BIT_STEREO;

    }

    //!Returns wav data frequency
    int cWavDecoder::getFrequency()
    {
        return mSampleRate;
    }

    //!Returns if seeking is supported
    bool cWavDecoder::isSeekingSupported()
    {
        return false;
    }

    //!Reads wav data
    int cWavDecoder::readAudioData(void* output, int amount)
    {
        return Stream->read(output,amount);

    }

    //!Sets data reader position
    bool cWavDecoder::setPosition(int position, bool relative)
    {
        Stream->seek(position,relative);
        return true;
    }

    //!Seeks wav data
    bool cWavDecoder::seek(float seconds,bool relative)
    {
        return false;
    }


}


