#ifndef CWAVAUDIODECODERFACOTRY_H_INCLUDED
#define CWAVAUDIODECODERFACOTRY_H_INCLUDED

#include "../include/IAudioDecoderFactory.h"
#include "cWavDecoder.h"
#include "../Headers/cMutex.h"

namespace cAudio
{

class cWavAudioDecoderFactory : public IAudioDecoderFactory
{
    public:
        cWavAudioDecoderFactory() {}
        ~cWavAudioDecoderFactory() {}

        IAudioDecoder* CreateAudioDecoder(IDataSource* stream)
        {
			Mutex.lock();
            IAudioDecoder* decoder = new cWavDecoder(stream);
			Mutex.unlock();
			return decoder;
        }
    protected:
		cAudioMutex Mutex;
    private:
};

};

#endif //! CWAVAUDIODECODERFACOTRY_H_INCLUDED
