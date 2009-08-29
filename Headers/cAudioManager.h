#ifndef CAUDIOMANAGER_H_INCLUDED
#define CAUDIOMANAGER_H_INCLUDED
#include <map>
#include <string>
#include <vector>
#include "cAudio.h"
#include "../include/IAudioDecoderFactory.h"
#include "cListener.h"
#include "cAudioCapture.h"
#include "../include/IAudioManager.h"
#include "../Headers/cMutex.h"

namespace cAudio
{
	class IAudio;

    class cAudioManager : public IAudioManager
    {
    public:
		//!Inits the audio manager calling the alut/etc start ups
		virtual void init(int argc,char* argv[]); 
		//!Shuts everything down        
		virtual void shutDown();
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
		//!Will delete the factory instance
		virtual void unRegisterAudioDecoder(std::string extension);
		//!Returns whether an audio decoder is currently registered for this file type
		virtual bool isAudioDecoderRegistered(std::string extension);
		//!Returns a registered audio decoder factory
		virtual IAudioDecoderFactory* getAudioDecoderFactory(std::string extension);

		//!Allows you to set the listener position (DEPRECIATED! USE getListener() INSTEAD!)       
		virtual void setListenerPos(float x,float y,float z);
		//!Set Listener Orientation (DEPRECIATED! USE getListener() INSTEAD!)
		virtual void setListenerOrientation(float ux,float uy,float uz);
		//!Updates the cAudio playback        
		virtual void update();
		//!Gets you the cAudio object you want
		virtual IAudio *getSound(std::string identifier);
		//!Releases "ALL" cAudio objects        
		virtual void release();

		virtual IListener* getListener() { return &initlistener; }
		virtual IAudioCapture* getAudioCapture() { return &initCapture; }

		virtual bool IsThreadRunning() { return RunThread; }

		static cAudioManager* Instance()
		{
			return &m_cAudioManager;
		}

	protected:
		cAudioManager() : RunThread(false){ }

	private:
		//Mutex for thread syncronization
		cAudioMutex Mutex;

		bool RunThread;

		//!Global cAudioManager
		static cAudioManager m_cAudioManager;
		//!The map that holds the cAudio objects
		std::map<std::string, IAudio*> audiomap; 
		//!Decoder map that holds all decoders by file extension
		std::map<std::string, IAudioDecoderFactory*> decodermap; 
		//!The listener object        
		cListener initlistener;
		//!The audio capture instance
		cAudioCapture initCapture;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
