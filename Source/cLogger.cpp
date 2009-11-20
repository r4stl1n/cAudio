#include "../Headers/cLogger.h"
#include <time.h>
#include "../Headers/cConsoleLogReceiver.h"

namespace cAudio
{
	static cLogger Logger;
	static bool FirstTimeLogInit(false);
	static cConsoleLogReceiver ConsoleLog;

	cLogger::cLogger() : StartTime(0), MinLogLevel(ELL_INFO)
	{
		StartTime = clock();
	}

	void cLogger::logCritical( const char* sender, const char *msg, ... )
	{
		if(ELL_CRITICAL >= MinLogLevel)
		{
			Mutex.lock();
			va_list args;
			va_start( args, msg );
			broadcastMessage( ELL_CRITICAL, sender, msg, args );
			va_end( args );
			Mutex.unlock();
		}
	}
	void cLogger::logError( const char* sender, const char *msg, ... )
	{
		if(ELL_ERROR >= MinLogLevel)
		{
			Mutex.lock();
			va_list args;
			va_start( args, msg );
			broadcastMessage( ELL_ERROR, sender, msg, args );
			va_end( args );
			Mutex.unlock();
		}
	}
	void cLogger::logWarning( const char* sender, const char *msg, ... )
	{
		if(ELL_WARNING >= MinLogLevel)
		{
			Mutex.lock();
			va_list args;
			va_start( args, msg );
			broadcastMessage( ELL_WARNING, sender, msg, args );
			va_end( args );
			Mutex.unlock();
		}
	}
	void cLogger::logInfo( const char* sender, const char *msg, ... )
	{
		if(ELL_INFO >= MinLogLevel)
		{
			Mutex.lock();
			va_list args;
			va_start( args, msg );
			broadcastMessage( ELL_INFO, sender, msg, args );
			va_end( args );
			Mutex.unlock();
		}
	}
	void cLogger::logDebug( const char* sender, const char *msg, ... )
	{
		if(ELL_DEBUG >= MinLogLevel)
		{
			Mutex.lock();
			va_list args;
			va_start( args, msg );
			broadcastMessage( ELL_DEBUG, sender, msg, args );
			va_end( args );
			Mutex.unlock();
		}
	}
	void cLogger::setLogLevel( const LogLevel& logLevel )
	{
		Mutex.lock();
		MinLogLevel = logLevel;
		Mutex.unlock();
	}
	void cLogger::broadcastMessage( LogLevel level, const char* sender, const char* msg, va_list args )
	{
		float messageTime = (clock() - StartTime) / (float)CLOCKS_PER_SEC;
		vsnprintf( TempTextBuf, 2048, msg, args );

		std::map<std::string,ILogReceiver*>::iterator it = Receivers.begin();
        for (it = Receivers.begin(); it != Receivers.end(); it++)
        {
            it->second->OnLogMessage(sender, TempTextBuf, level, messageTime);
        }
	}
	bool cLogger::registerLogReceiver(ILogReceiver* receiver, std::string name)
    {
		Mutex.lock();
        Receivers[name] = receiver;
		Mutex.unlock();
		return true;
    }

	void cLogger::unRegisterLogReceiver(std::string name)
	{
		Mutex.lock();
		std::map<std::string, ILogReceiver*>::iterator it = Receivers.find(name);
		if(it != Receivers.end())
		{
			Receivers.erase(it);
		}
		Mutex.unlock();
	}

	bool cLogger::isLogReceiverRegistered(std::string name)
	{
		Mutex.lock();
		std::map<std::string, ILogReceiver*>::iterator it = Receivers.find(name);
		bool result = (it != Receivers.end());
		Mutex.unlock();
		return result;
	}

	ILogReceiver* cLogger::getLogReceiver(std::string name)
	{
		Mutex.lock();
		std::map<std::string, ILogReceiver*>::iterator it = Receivers.find(name);
		if(it != Receivers.end())
		{
			Mutex.unlock();
			return it->second;
		}
		Mutex.unlock();
		return NULL;
	}

	CAUDIO_API ILogger* getLogger()
	{
		if(!FirstTimeLogInit)
		{
			FirstTimeLogInit = true;
			Logger.registerLogReceiver(&ConsoleLog, "Console");
		}
		return &Logger;
	}
};