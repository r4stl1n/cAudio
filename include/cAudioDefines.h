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

#ifdef CAUDIO_MAKE_THREAD_SAFE
//Define enables threading for the main update loop.  Disable to run the internal update loop yourself
//NOTE: Internal threading should NOT be used if the library is not thread safe!
#define CAUDIO_USE_INTERNAL_THREAD

#endif

#endif //! CAUDIODEFINES_H