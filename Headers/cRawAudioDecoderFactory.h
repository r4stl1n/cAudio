#ifndef CRAWAUDIODECODERFACOTRY_H_INCLUDED
#define CRAWAUDIODECODERFACOTRY_H_INCLUDED

#include "../include/IAudioDecoderFactory.h"
#include "cRawDecoder.h"


namespace cAudio
{

class cRawAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        cRawAudioDecoderFactory() {}
        ~cRawAudioDecoderFactory() {}

		IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
            return new cRawDecoder(stream, 22050, EAF_16BIT_MONO);
        }

		IAudioDecoder* CreateAudioDecoder(IDataSource* stream, unsigned int frequency = 22050, AudioFormats format = EAF_16BIT_MONO)
        {
            return new cRawDecoder(stream, frequency, format);
        }
    protected:
    private:
};

};

#endif //! CRAWAUDIODECODERFACOTRY_H_INCLUDED
