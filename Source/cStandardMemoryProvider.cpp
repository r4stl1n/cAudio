#include "../Headers/cStandardMemoryProvider.h"
#include <stdlib.h>
#include <limits>

namespace cAudio
{
	void* cStandardMemoryProvider::Allocate(size_t size, const char* filename, int line, const char* function)
	{
		return malloc(size);
	}

	void cStandardMemoryProvider::Free(void* pointer)
	{
		if(pointer)
		{
			free(pointer);
		}
	}

	size_t cStandardMemoryProvider::getMaxAllocationSize()
	{
		return std::numeric_limits<size_t>::max();
	}
};