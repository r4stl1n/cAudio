
#ifndef IAUDIOMANAGER_H
#define IAUDIOMANAGER_H
#include <string>
#include "IListener.h"

#ifndef CAUDIO_STATIC_LIB

#ifdef CAUDIO_EXPORTS
#define CAUDIO_API __declspec(dllexport)
#else
#define CAUDIO_API//! __declspec(dllimport)
#endif // CAUDIO_EXPORTS

#else
#define CAUDIO_API
#endif // CAUDIO_STATIC_LIB

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
		//!Creates the cAudio object
		virtual IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false) = 0;
		//!Loads audio from memory or virtual file system
		virtual IAudio* createFromMemory(const std::string& identifier,const char* data, size_t length, std::string ext) = 0;
		//!Register Audio Codec
		virtual void registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension) = 0;
		//!Allows you to set the listener position
		virtual void setListenerPos(float x,float y,float z) = 0;
		//!set the listeners orientation
		virtual void setListenerOrientation(float ux,float uy,float uz) = 0;
		//!Updates the cAudio playback
		virtual void update() = 0;
		//!Gets you the cAudio object you want
		virtual IAudio *getSound(std::string identifier) = 0;
		//!Releases "ALL" cAudio objects
		virtual void release() = 0;

		virtual IListener* getListener() = 0;

		virtual ~IAudioManager() {}
    protected:
    private:
    };

    CAUDIO_API IAudioManager* getAudioManager(void);
}
#endif //! IAUDIOMANAGER_H
