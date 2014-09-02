#pragma once

// CMake auto-generated configuration options

#define CAUDIO_STATIC_LIB 1

//! This define controls whether the Ogg/Vorbis decoder is compiled into the library.
#define CAUDIO_COMPILE_WITH_OGG_DECODER 1

//! This define controls whether the RIFF/Wav decoder is compiled into the library.
#define CAUDIO_COMPILE_WITH_WAV_DECODER 1

//! This define controls whether the default filesystem data source is compiled into the library
#define CAUDIO_COMPILE_WITH_FILE_SOURCE 1

//! This define controls whether the default file logger (html) is compiled into the library
#define CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER 1

//! This define controls whether the default console logger is compiled into the library
#define CAUDIO_COMPILE_WITH_CONSOLE_LOG_RECEIVER 0

//! Define for making the entire library Thread Safe, comment out to disable.  Will also disable internal threading by the library.
#define CAUDIO_MAKE_THREAD_SAFE 1

//! EFX support
#define CAUDIO_EFX_ENABLED 0

//! Tells cAudio to use the C standard memory functions for allocations (memalloc and free)
#define CAUDIO_MEMORY_USE_STD 1

//! Tells cAudio to reroute memory allocations from stl containers into the defined memory provider for cAudio, otherwise the standard std::allocator is used.
#define CAUDIO_REROUTE_STL_ALLOCATIONS 1

//! Activates the internal memory tracker, which can be used to detect and locate memory leaks.
#define CAUDIO_USE_MEMORYTRACKER 0

//! Tells the memory tracker to generate statistics on memory usage by cAudio
#define CAUDIO_MEMORYTRACKER_GENERATE_STATISTICS 0

//! Tells the memory tracker to log each and every allocation done by cAudio.  This can be very slow, use only if you are debugging an issue.
#define CAUDIO_MEMORYTRACKER_LOG_ALL_ALLOCATIONS 0
