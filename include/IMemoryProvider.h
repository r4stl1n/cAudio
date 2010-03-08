#ifndef IMEMORYPROVIDER_H_INCLUDED
#define IMEMORYPROVIDER_H_INCLUDED

#include "../include/cAudioDefines.h"

namespace cAudio
{
	//! Interface for a class that allocates and frees memory used by cAudio.
	class IMemoryProvider
	{
	public:
		virtual void* Allocate(size_t size, const char* filename, int line, const char* function) = 0;
		virtual void Free(void* pointer) = 0;
		virtual size_t getMaxAllocationSize() = 0;
	};
};

#endif //! IMEMORYPROVIDER_H_INCLUDED