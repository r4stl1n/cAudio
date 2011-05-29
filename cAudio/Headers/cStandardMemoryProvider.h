#pragma once

#include "../include/IMemoryProvider.h"
#include <cstring>
namespace cAudio
{
	//! Memory provider that wraps the standard memalloc and free
	class cStandardMemoryProvider : public IMemoryProvider
	{
	public:
		virtual void* Allocate(size_t size, const char* filename, int line, const char* function);
		virtual void Free(void* pointer);
		virtual size_t getMaxAllocationSize();
	};
};
