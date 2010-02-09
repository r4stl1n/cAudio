// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CAUDIODEFINES_H
#define CAUDIODEFINES_H

//To use EFX in linux.
#ifndef ALC_EXT_EFX
#define AL_FILTER_TYPE                                     0x8001
#define AL_EFFECT_TYPE                                     0x8001
#define AL_FILTER_NULL                                     0x0000
#define AL_FILTER_LOWPASS                                  0x0001
#define AL_FILTER_HIGHPASS                                 0x0002
#define AL_FILTER_BANDPASS                                 0x0003
#define AL_EFFECT_NULL                                     0x0000
#define AL_EFFECT_EAXREVERB                                0x8000
#define AL_EFFECT_REVERB                                   0x0001
#define AL_EFFECT_CHORUS                                   0x0002
#define AL_EFFECT_DISTORTION                               0x0003
#define AL_EFFECT_ECHO                                     0x0004
#define AL_EFFECT_FLANGER                                  0x0005
#define AL_EFFECT_FREQUENCY_SHIFTER                        0x0006
#define AL_EFFECT_VOCAL_MORPHER                            0x0007
#define AL_EFFECT_PITCH_SHIFTER                            0x0008
#define AL_EFFECT_RING_MODULATOR                           0x0009
#define AL_EFFECT_AUTOWAH                                  0x000A
#define AL_EFFECT_COMPRESSOR                               0x000B
#define AL_EFFECT_EQUALIZER                                0x000C
#define ALC_EFX_MAJOR_VERSION                              0x20001
#define ALC_EFX_MINOR_VERSION                              0x20002
#define ALC_MAX_AUXILIARY_SENDS                            0x20003
#endif
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