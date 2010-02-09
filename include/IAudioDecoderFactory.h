// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IAUDIODECODERFACTORY_H
#define IAUDIODECODERFACTORY_H

#include "IAudioDecoder.h"

namespace cAudio
{

class IAudioDecoderFactory
{
    public:
        IAudioDecoderFactory() {}
        virtual ~IAudioDecoderFactory() {}

        virtual IAudioDecoder* CreateAudioDecoder(IDataSource* stream) = 0;
    protected:
    private:
};

};

#endif //! IAUDIODECODERFACTORY_H
