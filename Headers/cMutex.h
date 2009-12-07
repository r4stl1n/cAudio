#ifndef CAUDIOMUTEX_H
#define CAUDIOMUTEX_H

#include "../include/cAudioDefines.h"

#ifdef CAUDIO_MAKE_THREAD_SAFE
	#ifdef _WIN32
	#include <windows.h>	//Basic windows include
	#else
	#include <pthread.h>	//Assumed linux system
	#endif
#endif

namespace cAudio
{
	//Basic mutex class used for internal thread locking
#ifdef CAUDIO_MAKE_THREAD_SAFE
	class cAudioMutex
	{
	public:
		cAudioMutex();
		~cAudioMutex();

		void lock();
		void unlock();
	private:
		void initialize();
		#ifdef _WIN32
		CRITICAL_SECTION criticalSection;
		#else
		pthread_mutex_t Mutex;
		#endif
		bool Initialized;
	};
#else
	//Dud class to disable the mutex
	class cAudioMutex
	{
	public:
		cAudioMutex();
		~cAudioMutex();

		void lock();
		void unlock();
	private:
		void initialize();
		bool Initialized;
	};
#endif

#ifdef CAUDIO_MAKE_THREAD_SAFE
	class cAudioMutexBasicLock
	{
	public:
		cAudioMutexBasicLock(cAudioMutex& mutex) : Mutex(&mutex)
		{
			Mutex->lock();
		}
		~cAudioMutexBasicLock()
		{
			Mutex->unlock();
		}
	protected:
		cAudioMutex* Mutex;
	};
#endif
};

#endif //! CAUDIOMUTEX_H