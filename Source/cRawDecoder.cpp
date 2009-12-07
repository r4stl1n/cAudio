#include "../Headers/cRawDecoder.h"

namespace cAudio{

	cRawDecoder::cRawDecoder(IDataSource* stream,  unsigned int frequency, AudioFormats format) : IAudioDecoder(stream), Frequency(frequency), Format(format)
    {

    }

    cRawDecoder::~cRawDecoder()
    {

    }

    //!Returns wav channel format
    AudioFormats cRawDecoder::getFormat()
    {
        return Format;
    }

    //!Returns wav data frequency
    int cRawDecoder::getFrequency()
    {
        return Frequency;
    }

    //!Returns if seeking is supported
    bool cRawDecoder::isSeekingSupported()
    {
        return true;
    }

	bool cRawDecoder::isValid()
	{
		return true;
	}

    //!Reads wav data
    int cRawDecoder::readAudioData(void* output, int amount)
    {
        return Stream->read(output,amount);
    }

    //!Sets data reader position
    bool cRawDecoder::setPosition(int position, bool relative)
    {
        Stream->seek(position,relative);
        return true;
    }

    //!Seeks wav data
    bool cRawDecoder::seek(float seconds,bool relative)
    {
		int SampleSize = 1;
		if(Format == EAF_8BIT_MONO)
			SampleSize = 1;
		else if(Format == EAF_8BIT_STEREO)
			SampleSize = 2;
		else if(Format == EAF_16BIT_MONO)
			SampleSize = 2;
		else
			SampleSize = 4;

        int amountToSeek = seconds * (float)Frequency * (float)SampleSize;
        return setPosition(amountToSeek, relative);
    }
}
