#ifndef CAUDIODEFINES_H
#define CAUDIODEFINES_H

#include "cAudioPlatform.h"

//Global define for exporting the library
#ifndef CAUDIO_STATIC_LIB

#ifdef CAUDIO_EXPORTS
	#define CAUDIO_API __declspec(dllexport)
#else
	#define CAUDIO_API//! __declspec(dllimport)
#endif // CAUDIO_EXPORTS

#else
	#define CAUDIO_API
#endif // CAUDIO_STATIC_LIB

//////////////////////////
//Global Library Settings
//////////////////////////

//Define for making the entire library Thread Safe, comment out to disable
//Will also disable internal threading by the library
#define CAUDIO_MAKE_THREAD_SAFE

#ifdef CAUDIO_MAKE_THREAD_SAFE
	//Define enables threading for the main update loop.  Disable to run the internal update loop yourself
	//NOTE: Internal threading should NOT be used if the library is not thread safe!
	#define CAUDIO_USE_INTERNAL_THREAD
#endif

//These defines control which of the default audio codecs are compiled into the library
#define CAUDIO_COMPILE_WITH_OGG_DECODER
#define CAUDIO_COMPILE_WITH_WAV_DECODER
#define CAUDIO_COMPILE_WITH_RAW_DECODER

////////////////////////
//Audio Source Settings
////////////////////////

//Size of the internal buffer per source for audio data (total amount buffered is CAUDIO_SOURCE_BUFFER_SIZE * CAUDIO_SOURCE_NUM_BUFFERS)
#define CAUDIO_SOURCE_BUFFER_SIZE ( 1024 * 64 )
//Number of internal buffers to cycle through per source (Note: using only 1 leads to choppy sound or premature ending of sources)
#define CAUDIO_SOURCE_NUM_BUFFERS 3

/////////////////////////
//Audio Effects Settings
/////////////////////////

//Comment out to remove all EFX support from the library
#define CAUDIO_EFX_ENABLED

#ifdef CAUDIO_EFX_ENABLED
	//Max number of effects that can be attached to a single sound source
	#define CAUDIO_SOURCE_MAX_EFFECT_SLOTS 4
#endif

#endif //! CAUDIODEFINES_H