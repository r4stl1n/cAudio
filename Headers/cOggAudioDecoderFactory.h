#ifndef COGGAUDIODECODERFACTORY_H
#define COGGAUDIODECODERFACTORY_H

#include "../include/IAudioDecoderFactory.h"
#include "cOggDecoder.h"

namespace cAudio
{

class cOggAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        cOggAudioDecoderFactory() {}
        ~cOggAudioDecoderFactory() {}

        IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
            return new cOggDecoder(stream);
        }

    protected:
    private:
};

};

#endif //! COGGAUDIODECODERFACTORY_H
