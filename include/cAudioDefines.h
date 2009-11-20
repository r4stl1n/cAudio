#ifndef CAUDIODEFINES_H
#define CAUDIODEFINES_H

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

//Define for making the entire library Thread Safe, comment out to disable
//Will also disable internal threading by the library
#define CAUDIO_MAKE_THREAD_SAFE

//Define for compiling with EAX
#define CAUDIO_EAX_ENABLED

#ifdef CAUDIO_MAKE_THREAD_SAFE
//Define enables threading for the main update loop.  Disable to run the internal update loop yourself
//NOTE: Internal threading should NOT be used if the library is not thread safe!
#define CAUDIO_USE_INTERNAL_THREAD

#endif

//These defines control which of the default audio codecs are compiled into the library
#define CAUDIO_COMPILE_WITH_OGG_DECODER
#define CAUDIO_COMPILE_WITH_WAV_DECODER
#define CAUDIO_COMPILE_WITH_RAW_DECODER

#endif //! CAUDIODEFINES_H