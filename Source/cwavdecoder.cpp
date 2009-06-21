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
        mSampleRate += mSampleRate;
    }

    cWavDecoder::~cWavDecoder()
    {
        mChunkSize = NULL;
        mSubChunk1Size = NULL;
        mFormat = NULL;
        mChannels = NULL;
        mSampleRate = NULL;
        mByteRate = NULL;
        mBlockAlign = NULL;
        mBitsPerSample = NULL;
        mDataSize = NULL;

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
    bool cWavDecoder::seek(int seconds,bool relative)
    {
        return false;
    }


}


