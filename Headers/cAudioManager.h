#ifndef CAUDIOMANAGER_H_INCLUDED
#define CAUDIOMANAGER_H_INCLUDED
#include <map>
#include <string>
#include <vector>
#include "cAudio.h"
#include "../include/IAudioDecoderFactory.h"
#include "cListener.h"
#include "../include/IAudioManager.h"
#include "../Headers/cMutex.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace cAudio
{
	class IAudio;

    class cAudioManager : public IAudioManager
    {
    public:
		cAudioManager() { }
		virtual ~cAudioManager() { }

		//!Inits the audio manager calling the alut/etc start ups
		virtual bool initialize(const char* deviceName = 0x0, int outputFrequency = -1, int eaxEffectSlots = 4); 
		//!Shuts everything down        
		virtual void shutDown();

		//! Returns the name of an available playback device.
		/** \param index: Specify which name to retrieve ( Range: 0 to getAvailableDeviceCount()-1 ) */
		virtual const char* getAvailableDeviceName(unsigned int index);
		//! Returns the number of playback devices available for use.
		virtual unsigned int getAvailableDeviceCount();
		//! Returns the name of the default system playback device.
		virtual const char* getDefaultDeviceName();

		//!Creates the cAudio object
		virtual IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false);
		//!Loads ogg from memory or virtual file system
		virtual IAudio* createFromMemory(const std::string& identifier, const char* data, size_t length, std::string ext);
		//!Loads raw audio from memory.
		virtual IAudio* createFromRaw(const std::string& identifier,const char* data, size_t length, unsigned int frequency, AudioFormats format);
		
		//!Register Audio Codec        
		virtual bool registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension);
		//!Unregister Audio Codec (allows you to prevent an file type from being playable with new sound sources)
		//!Note that all current sound sources will still continue to use any currently allocated decoders.
		//!Will NOT delete any user added factory instance, you must do that yourself
		virtual void unRegisterAudioDecoder(std::string extension);
		//!Returns whether an audio decoder is currently registered for this file type
		virtual bool isAudioDecoderRegistered(std::string extension);
		//!Returns a registered audio decoder factory
		virtual IAudioDecoderFactory* getAudioDecoderFactory(std::string extension);

		//!Updates the cAudio playback        
		virtual void update();
		//!Gets you the cAudio object you want
		virtual IAudio *getSound(std::string identifier);
		//!Releases "ALL" cAudio objects        
		virtual void release();

		//! Grabs a list of available devices, as well as the default system one
		void getAvailableDevices();

		virtual IListener* getListener() { return &initlistener; }

	private:
		//Mutex for thread syncronization
		cAudioMutex Mutex;

		//Make a Openal context pointer
		ALCcontext *Context;
		//Make a openal device pointer
		ALCdevice *Device;

		//!The map that holds the cAudio objects
		std::map<std::string, IAudio*> audiomap; 
		//!Decoder map that holds all decoders by file extension
		std::map<std::string, IAudioDecoderFactory*> decodermap; 
		//!The listener object        
		cListener initlistener;
		//! Check for OpenAL errors
		void checkError();

		std::vector<std::string> AvailableDevices;
		std::string DefaultDevice;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
