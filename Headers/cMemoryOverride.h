#ifndef CMEMORYOVERRIDE_H_INCLUDED
#define CMEMORYOVERRIDE_H_INCLUDED

#include <new>
#include "../include/cAudioMemory.h"

#ifdef CAUDIO_DEBUG
	#define CAUDIO_NEW new (__FILE__, __LINE__, __FUNCTION__)
	#define CAUDIO_DELETE delete
	#define CAUDIO_MALLOC(size) cAudio::getMemoryProvider()->Allocate(size, __FILE__, __LINE__, __FUNCTION__)
	#define CAUDIO_FREE(pointer) cAudio::getMemoryProvider()->Free((void*)pointer)
#else
	#define CAUDIO_NEW new 
	#define CAUDIO_DELETE delete
	#define CAUDIO_MALLOC(size) cAudio::getMemoryProvider()->Allocate(size, NULL, -1, NULL)
	#define CAUDIO_FREE(pointer) cAudio::getMemoryProvider()->Free((void*)pointer)
#endif

namespace cAudio
{
	class cMemoryOverride
	{
	public:
		void* operator new(size_t size, const char* file, int line, const char* function)
		{
			return cAudio::getMemoryProvider()->Allocate(size, file, line, function);
		}

		void* operator new(size_t size)
		{
			return cAudio::getMemoryProvider()->Allocate(size, NULL, -1, NULL);
		}

		void* operator new(size_t size, void* pointer)
		{
			(void) size;
			return pointer;
		}

		void* operator new[] ( size_t size, const char* file, int line, const char* function )
		{
			return cAudio::getMemoryProvider()->Allocate(size, file, line, function);
		}

		void* operator new[] ( size_t size )
		{
			return cAudio::getMemoryProvider()->Allocate(size, NULL, -1, NULL);
		}

		void operator delete( void* pointer )
		{
			cAudio::getMemoryProvider()->Free(pointer);
		}

		void operator delete( void* pointer, void* )
		{
			cAudio::getMemoryProvider()->Free(pointer);
		}

		void operator delete( void* pointer, const char* , int , const char*  )
		{
			cAudio::getMemoryProvider()->Free(pointer);
		}

		void operator delete[] ( void* pointer )
		{
			cAudio::getMemoryProvider()->Free(pointer);
		}

		void operator delete[] ( void* pointer, const char* , int , const char*  )
		{
			cAudio::getMemoryProvider()->Free(pointer);
		}
	};
};

#endif //! CMEMORYOVERRIDE_H_INCLUDED
