#include "../Headers/cLogger.h"

namespace cAudio{

cLogger::cLogger()
{
	Mutex.lock();
	logLevel = E_LOGLEVEL_4;
	Mutex.unlock();
}

cLogger::~cLogger()
{
	Mutex.lock();
	
	Mutex.unlock();
}

void cLogger::setLogLevel(E_LOGLEVEL level)
{
	Mutex.lock();
	logLevel = level;
	Mutex.unlock();
}

void cLogger::log(E_LOGLEVEL loglevel,const char* text,...)
{
    Mutex.lock();
    char buffer[ 4096 ];
    va_list marker;
    va_start( marker, text );
    vsprintf( buffer, text, marker );
    va_end( marker );
    if(logLevel <= loglevel)
    {
        std::cout<< buffer <<std::endl;
    }
    Mutex.unlock(); 
}

}