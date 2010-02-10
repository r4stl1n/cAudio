// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CAUDIOMANAGER_H_INCLUDED
#define CAUDIOMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "cAudioSource.h"
#include "../include/IAudioDecoderFactory.h"
#include "cListener.h"
#include "../include/IAudioManager.h"
#include "../Headers/cMutex.h"
#include "../Headers/cAudioEffects.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace cAudio
{
	class IAudioSource;

    class cAudioManager : public IAudioManager
    {
    public:
		cAudioManager() : Device(NULL), Context(NULL), EFXSupported(false), Initialized(false) { }
		virtual ~cAudioManager() { }

		//!Inits the audio manager calling the alut/etc start ups
		virtual bool initialize(const char* deviceName = 0x0, int outputFrequency = -1, int eaxEffectSlots = 4); 
		//!Shuts everything down        
		virtual void shutDown();
		//!Updates the cAudio playback        
		virtual void update();
		//!Gets you the cAudio object you want
		virtual IAudioSource* getSoundByName(const char* name);
		//!Releases "ALL" cAudio objects        
		virtual void release();
		//!Releases a single cAudio source
		virtual void release(IAudioSource* source);

		//! Returns the name of an available playback device.
		/** \param index: Specify which name to retrieve ( Range: 0 to getAvailableDeviceCount()-1 ) */
		virtual const char* getAvailableDeviceName(unsigned int index);
		//! Returns the number of playback devices available for use.
		virtual unsigned int getAvailableDeviceCount();
		//! Returns the name of the default system playback device.
		virtual const char* getDefaultDeviceName();

		//!Creates the cAudio object
		virtual IAudioSource* createFromFile(const char* name, const char* pathToFile, bool stream = false);
		//!Loads ogg from memory or virtual file system
		virtual IAudioSource* createFromMemory(const char* name, const char* data, size_t length, const char* extension);
		//!Loads raw audio from memory.
		virtual IAudioSource* createFromRaw(const char* name, const char* data, size_t length, unsigned int frequency, AudioFormats format);
		
		//!Register Audio Codec        
		virtual bool registerAudioDecoder(IAudioDecoderFactory* factory, const char* extension);
		//!Unregister Audio Codec (allows you to prevent an file type from being playable with new sound sources)
		//!Note that all current sound sources will still continue to use any currently allocated decoders.
		//!Will NOT delete any user added factory instance, you must do that yourself
		virtual void unRegisterAudioDecoder(const char* extension);
		//!Returns whether an audio decoder is currently registered for this file type
		virtual bool isAudioDecoderRegistered(const char* extension);
		//!Returns a registered audio decoder factory
		virtual IAudioDecoderFactory* getAudioDecoderFactory(const char* extension);

		//! Grabs a list of available devices, as well as the default system one
		void getAvailableDevices();

		virtual IListener* getListener() { return &initlistener; }
		virtual IAudioEffects* getEffects() { return &initEffects; }

	private:
		//Mutex for thread syncronization
		cAudioMutex Mutex;

		//Make a Openal context pointer
		ALCcontext* Context;
		//Make a openal device pointer
		ALCdevice* Device;

		//Holds whether EFX is supported
		bool EFXSupported;

		//Whether the manager is currently initialized and ready to go.
		bool Initialized;

		//! Holds an index for fast searching of audio sources by name
		std::map<std::string, IAudioSource*> audioIndex;
		//! Holds all managed audio sources
		std::vector<IAudioSource*> audioSources;
		//! Decoder map that holds all decoders by file extension
		std::map<std::string, IAudioDecoderFactory*> decodermap; 
		//! The listener object        
		cListener initlistener;
		//! Interface for audio effects
		cAudioEffects initEffects;
		//! Check for OpenAL errors
		bool checkError();

		std::vector<std::string> AvailableDevices;
		std::string DefaultDevice;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
