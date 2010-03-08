#ifndef CSTANDARDMEMORYPROVIDER_H_INCLUDED
#define CSTANDARDMEMORYPROVIDER_H_INCLUDED

#include "../include/IMemoryProvider.h"

namespace cAudio
{
	class cStandardMemoryProvider : public IMemoryProvider
	{
	public:
		virtual void* Allocate(size_t size, const char* filename, int line, const char* function);
		virtual void Free(void* pointer);
		virtual size_t getMaxAllocationSize();
	};
};

#endif //! CSTANDARDMEMORYPROVIDER_H_INCLUDED
