#include "cMP3Decoder.h"

cMP3Decoder::cMP3Decoder(IDataSource* stream) : IAudioDecoder(stream)
{

}

cMP3Decoder::~cMP3Decoder()
{

}

AudioFormats cMP3Decoder::getFormat()
{
	return EAF_16BIT_STEREO;
}

int cMP3Decoder::getFrequency()
{
	return 0;
}

bool cMP3Decoder::isSeekingSupported()
{
	return false;
}

bool cMP3Decoder::isValid()
{
	return false;
}

int cMP3Decoder::readAudioData(void* output, int amount)
{
	return 0;
}

bool cMP3Decoder::setPosition(int position, bool relative)
{
	return false;
}

bool cMP3Decoder::seek(float seconds, bool relative)
{
	return false;
}
