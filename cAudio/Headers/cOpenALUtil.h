
#ifndef CAUDIO_COPENALUTIL_H
#define CAUDIO_COPENALUTIL_H

#include "cAudio.h"

#if !defined(CAUDIO_PLATFORM_LINUX)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#if DEBUG
#ifndef __func__
#define __func__ __FUNCTION__
#endif
#define checkALError() checkALErrorInternal(__FILE__, __func__, __LINE__)
#else
#define checkALError() (false)
#endif

namespace cAudio {

    //! Checks for OpenAL errors and reports them
    inline bool checkALErrorInternal(const char* file, const char *func, int line)
    {
        ALenum error = AL_NO_ERROR;
        bool anyError = false;
        while ((error = alGetError()) != AL_NO_ERROR)
        {
			const char* errorString = alGetString(error);
			if(error == AL_OUT_OF_MEMORY) {
				getLogger()->logCritical("Audio Source", "OpenAL Error: %s:%d:%s, %s.", file, line, func, errorString);
            } else if (error == -1) {
                // this occurs randomly and relatively freqently with Apple's OpenAL and does not seem to cause serious problems.
                getLogger()->logWarning("Audio Source", "OpenAL Warning %s:%d:%s, %s.", file, line, func, errorString);
			} else {
				getLogger()->logError("Audio Source", "OpenAL Error: %s:%d:%s, %s.", file, line, func, errorString);
            }
            anyError = true;
        }
		return anyError;
    }

    //! Converts our audio format enum to OpenAL's
	inline ALenum convertAudioFormatEnum(AudioFormats format)
	{
		switch(format)
		{
		case EAF_8BIT_MONO:
			return AL_FORMAT_MONO8;
		case EAF_16BIT_MONO:
			return AL_FORMAT_MONO16;
		case EAF_8BIT_STEREO:
			return AL_FORMAT_STEREO8;
		case EAF_16BIT_STEREO:
			return AL_FORMAT_STEREO16;
		default:
			return AL_FORMAT_MONO8;
		};
	}

}

#endif


