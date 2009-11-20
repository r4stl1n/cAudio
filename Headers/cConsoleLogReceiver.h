#ifndef CCONSOLELOGRECEIVER_H_INCLUDED
#define CCONSOLELOGRECEIVER_H_INCLUDED

#include "../include/ILogReceiver.h"

namespace cAudio
{

    class cConsoleLogReceiver : public ILogReceiver
    {
        public:
			virtual bool OnLogMessage(const char* sender, const char* message, LogLevel level, float time);
        private:
    };

};

#endif //! CCONSOLELOGRECEIVER_H_INCLUDED

