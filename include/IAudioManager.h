#ifndef IAUDIOMANAGER_H
#define IAUDIOMANAGER_H
#include <string>
#include "IListener.h"
#include "cAudioDefines.h"
#include "eAudioFormats.h"

namespace cAudio
{
	class IAudio;
	class IAudioDecoderFactory;

    class IAudioManager
    {
    public:
		IAudioManager() { }
		virtual ~IAudioManager() { }

		//!Inits the audio manager calling the alut/etc start ups
		virtual bool initialize(const char* deviceName = 0x0, int outputFrequency = -1, int eaxEffectSlots = 4) = 0;
		//!Shuts everything down
		virtual void shutDown() = 0;
		//!Updates the cAudio playback
		virtual void update() = 0;
		//!Returns an IAudio object by its "name" and 0 if the name is not found
		virtual IAudio* getSound(std::string identifier) = 0;
		//!Releases "ALL" cAudio objects (but does not shutdown the manager)
		virtual void release() = 0;

		//! Returns the name of an available playback device.
		/** \param index: Specify which name to retrieve ( Range: 0 to getAvailableDeviceCount()-1 ) */
		virtual const char* getAvailableDeviceName(unsigned int index) = 0;
		//! Returns the number of playback devices available for use.
		virtual unsigned int getAvailableDeviceCount() = 0;
		//! Returns the name of the default system playback device.
		virtual const char* getDefaultDeviceName() = 0;
		
		//!Creates the cAudio object
		virtual IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false) = 0;
		//!Loads audio from memory or virtual file system
		virtual IAudio* createFromMemory(const std::string& identifier,const char* data, size_t length, std::string ext) = 0;
		//!Loads raw audio from memory.
		virtual IAudio* createFromRaw(const std::string& identifier,const char* data, size_t length, unsigned int frequency, AudioFormats format) = 0;

		//!Register Audio Codec
		virtual bool registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension) = 0;
		//!Unregister Audio Codec (allows you to prevent an file type from being playable with new sound sources)
		//!Note that all current sound sources will still continue to use any currently allocated decoders.
		//!Will NOT delete any user added factory instance, you must do that yourself
		virtual void unRegisterAudioDecoder(std::string extension) = 0;
		//!Returns whether an audio decoder is currently registered for this file type
		virtual bool isAudioDecoderRegistered(std::string extension) = 0;
		//!Returns a registered audio decoder factory
		virtual IAudioDecoderFactory* getAudioDecoderFactory(std::string extension) = 0;

		//!Returns an interface for the listener
		virtual IListener* getListener() = 0;

    protected:
    private:
    };

    //! Creates an interface to an Audio Manager
	/** Note: This is the only way to get access to the audio playback capabilities of cAudio.
	You must delete this interface using destroyAudioManager() once you are done with it.
	\param initializeDefault: Whether to return an object initialized with the default settings.  If set to false, you must make a call to initialize before you can create audio sources
	\return A pointer to the created object, NULL if the object could not be allocated.
	*/
	CAUDIO_API IAudioManager* createAudioManager(bool initializeDefault = true);

	//! Destroys an interface to a previously created Audio Manager and frees the memory allocated for it
	/** 
	\param capture: The object to destroy
	*/
	CAUDIO_API void destroyAudioManager(IAudioManager* manager);

	//! Returns if the thread used to update all Audio Managers is running
	/** Note: Will always return false if threading is disabled.
	The library automatically shuts down the thread if no Audio Managers exist and will restart the thread on creation of a new manager.
	\return True if the thread is currently running, false otherwise.
	*/
	CAUDIO_API bool isAudioManagerThreadRunning();
}
#endif //! IAUDIOMANAGER_H
