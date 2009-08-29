#ifndef IAUDIOMANAGER_H
#define IAUDIOMANAGER_H
#include <string>
#include "IListener.h"
#include "IAudioCapture.h"
#include "cAudioDefines.h"

namespace cAudio
{
	class IAudio;
	class IAudioDecoderFactory;

    class IAudioManager
    {
    public:
		//!Inits the audio manager calling the alut/etc start ups
		virtual void init(int argc,char* argv[]) = 0;
		//!Shuts everything down
		virtual void shutDown() = 0;
		//!Updates the cAudio playback
		virtual void update() = 0;
		//!Returns an IAudio object by its "name" and 0 if the name is not found
		virtual IAudio *getSound(std::string identifier) = 0;
		//!Releases "ALL" cAudio objects (but does not shutdown the manager)
		virtual void release() = 0;
		
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
		virtual void unRegisterAudioDecoder(std::string extension) = 0;
		//!Returns whether an audio decoder is currently registered for this file type
		virtual bool isAudioDecoderRegistered(std::string extension) = 0;
		//!Returns a registered audio decoder factory
		virtual IAudioDecoderFactory* getAudioDecoderFactory(std::string extension) = 0;

		//!Allows you to set the listener position (DEPRECIATED! USE getListener() INSTEAD!)
		virtual void setListenerPos(float x,float y,float z) = 0;
		//!set the listeners orientation (DEPRECIATED! USE getListener() INSTEAD!)
		virtual void setListenerOrientation(float ux,float uy,float uz) = 0;

		//!Returns an interface for the listener
		virtual IListener* getListener() = 0;
		//!Returns an interface for audio capture
		virtual IAudioCapture* getAudioCapture() = 0;

		virtual bool IsThreadRunning() = 0;

		virtual ~IAudioManager() {}
    protected:
    private:
    };

    CAUDIO_API IAudioManager* getAudioManager(void);
}
#endif //! IAUDIOMANAGER_H
