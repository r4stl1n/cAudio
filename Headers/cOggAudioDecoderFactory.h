#ifndef COGGAUDIODECODERFACTORY_H
#define COGGAUDIODECODERFACTORY_H

#include "../include/IAudioDecoderFactory.h"
#include "cOggDecoder.h"
#include "../Headers/cMutex.h"

namespace cAudio
{

class cOggAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        cOggAudioDecoderFactory() {}
        ~cOggAudioDecoderFactory() {}

        IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
			Mutex.lock();
			IAudioDecoder* decoder = new cOggDecoder(stream);
			Mutex.unlock();
            return decoder;
        }

    protected:
		cAudioMutex Mutex;
    private:
};

};

#endif //! COGGAUDIODECODERFACTORY_H
