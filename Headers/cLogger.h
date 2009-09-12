#ifndef CLOGGERR_H_INCLUDED
#define CLOGGER_H_INCLUDED
#include <iostream>
#include "../Include/ILogger.h"
#include "../Headers/cMutex.h"

namespace cAudio{

class cLogger : public ILogger{

	public:
		cLogger();
		virtual ~cLogger();

		void setLogLevel(E_LOGLEVEL level);
		void log(E_LOGLEVEL loglevel,const char* text,...);

	private:
		E_LOGLEVEL logLevel;
		//Mutex for thread syncronization
		cAudioMutex Mutex;
		
};

}
#endif