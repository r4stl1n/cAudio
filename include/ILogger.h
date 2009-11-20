#ifndef ILOGGER_H_INCLUDED
#define ILOGGER_H_INCLUDED

#include "ILogReceiver.h"
#include "cAudioDefines.h"
#include <string>

namespace cAudio
{
	class ILogger
    {
    public:
		ILogger() { }
		virtual ~ILogger() { }

		virtual void logCritical( const char* sender, const char *msg, ... ) = 0;
		virtual void logError( const char* sender, const char *msg, ... ) = 0;
		virtual void logWarning( const char* sender, const char *msg, ... ) = 0;
		virtual void logInfo( const char* sender, const char *msg, ... ) = 0;
		virtual void logDebug( const char* sender, const char *msg, ... ) = 0;

		virtual const LogLevel& getLogLevel() const = 0;
		virtual void setLogLevel( const LogLevel& logLevel ) = 0;

		//! Register Log Receiver
		//! Note: Any class registered will become owned by the internal thread.
		//! If threading is enabled, you MUST make the receiver threadsafe if you plan to access it in your application while it is registered
		virtual bool registerLogReceiver(ILogReceiver* receiver, std::string name) = 0;
		//!Unregister a Log Receiver
		//!Will NOT delete any user added receiver, you must do that yourself
		virtual void unRegisterLogReceiver(std::string name) = 0;
		//!Returns whether an log receiver is currently registered
		virtual bool isLogReceiverRegistered(std::string name) = 0;
		//!Returns a registered log receiver
		virtual ILogReceiver* getLogReceiver(std::string name) = 0;
	protected:
	private:
    };

	//! Gets the interface to the logger
	/** Note: This is the only way to get access to the logging capabilities of cAudio.
	\return A pointer to the object
	*/
	CAUDIO_API ILogger* getLogger();
};
#endif //! ILOGGER_H_INCLUDED
