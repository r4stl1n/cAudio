#ifndef ICAUDIOLOGGER_H_INCLUDED
#define ICAUDIOLOGGER_H_INCLUDED
#include <iostream>
#include <string>

namespace cAudio{

	enum E_LOGLEVEL{
		E_LOGLEVEL_1,
		E_LOGLEVEL_2,
		E_LOGLEVEL_3,
		E_LOGLEVEL_4,

	};

	class ILogger{

	public:
		ILogger(){}
		virtual ~ILogger(){}
		virtual void setLogLevel(E_LOGLEVEL level) = 0;
		virtual void log(E_LOGLEVEL loglevel,const char* text,...) = 0;
		
	};
}

#endif