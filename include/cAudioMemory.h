#ifndef CAUDIOMEMORY_H_INCLUDED
#define CAUDIOMEMORY_H_INCLUDED

#include "cAudioDefines.h"
#include "IMemoryProvider.h"

namespace cAudio
{
	CAUDIO_API IMemoryProvider* getMemoryProvider();
};

#endif //! CAUDIOMEMORY_H_INCLUDED