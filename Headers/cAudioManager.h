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
		enum Events{
			ON_INIT,
			ON_UPDATE,
			ON_RELEASE,
			ON_SOURCECREATE,
			ON_DECODERREGISTER,
			ON_DATASOURCEREGISTER,
		};

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
		virtual void releaseAllSources();
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
		virtual IAudioSource* create(const char* name, const char* filename, bool stream = false);
		//!Loads data from memory or virtual file system
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
		//!Unregisters all Audio Codecs
		//!Note that all current sound sources will still continue to use any currently allocated decoders.
		//!Will NOT delete any user added factory instance, you must do that yourself
		virtual void unRegisterAllAudioDecoders();

		virtual bool registerDataSource(IDataSourceFactory* factory, const char* name, int priority);
		virtual void unRegisterDataSource(const char* name);
		virtual bool isDataSourceRegistered(const char* name);
		virtual IDataSourceFactory* getDataSourceFactory(const char* name);
		virtual void unRegisterAllDataSources();

		//!Registers a new event handler to the manager
		virtual void registerEventHandler(IManagerEventHandler* handler);
		//!Unregisters specified event handler from manager
		virtual void unRegisterEventHandler(IManagerEventHandler* handler) ;
		//!Unregisters all event handlers attached to the manager
		virtual void unRegisterAllEventHandlers();

		//! Grabs a list of available devices, as well as the default system one
		void getAvailableDevices();

		virtual IListener* getListener() { return &initlistener; }

#ifdef CAUDIO_EFX_ENABLED
		virtual IAudioEffects* getEffects() { return &initEffects; }
#endif

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
		//! Archive map that holds all datasource types
		std::map<std::string, IDataSourceFactory*> datasourcemap;
		std::vector< std::pair<int, std::string> > dataSourcePriorityList;

		//! The listener object        
		cListener initlistener;
#ifdef CAUDIO_EFX_ENABLED
		//! Interface for audio effects
		cAudioEffects initEffects;
#endif
		//! Check for OpenAL errors
		bool checkError();

		std::vector<std::string> AvailableDevices;
		std::string DefaultDevice;

		//Signals a event to all event handlers
		void signalEvent(Events sevent);
		
		//! List of all attached event handlers
		std::list<IManagerEventHandler*> eventHandlerList;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
