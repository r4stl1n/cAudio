#include "../Headers/cMutex.h"

namespace cAudio
{
#ifdef CAUDIO_MAKE_THREAD_SAFE
	cAudioMutex::cAudioMutex() : Initialized(false)
	{
	}

	cAudioMutex::~cAudioMutex()
	{
		if(!Initialized)
			return;
		#ifdef _WIN32
		DeleteCriticalSection(&criticalSection);
		#else
		pthread_mutex_destroy(&Mutex);
		#endif
	}

	void cAudioMutex::lock(void)
	{
		if(!Initialized)
			initialize();

	#ifdef _WIN32
		EnterCriticalSection(&criticalSection);
	#else
		int error = pthread_mutex_lock(&Mutex);
	#endif
	}

	void cAudioMutex::unlock(void)
	{
		if(!Initialized)
			return;
	#ifdef _WIN32
		LeaveCriticalSection(&criticalSection);
	#else
		int error = pthread_mutex_unlock(&Mutex);
	#endif
	}

	void cAudioMutex::initialize(void)
	{
	#ifdef _WIN32
		InitializeCriticalSection(&criticalSection);
	#else
		pthread_mutexattr_t attr;
		int error = pthread_mutexattr_init(&attr);
		error = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
		error = pthread_mutex_init(&Mutex, &attr);
		error = pthread_mutexattr_destroy(&attr);
	#endif
		Initialized=true;
	}
#else
	cAudioMutex::cAudioMutex() : Initialized(false)
	{
	}

	cAudioMutex::~cAudioMutex()
	{
		//Do nothing
	}

	void cAudioMutex::lock(void)
	{
		//Do nothing
	}

	void cAudioMutex::unlock(void)
	{
		//Do nothing
	}

	void cAudioMutex::initialize(void)
	{
		//Do nothing
	}
#endif

};