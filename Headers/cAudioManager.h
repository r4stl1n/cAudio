#ifndef CAUDIOMANAGER_H_INCLUDED
#define CAUDIOMANAGER_H_INCLUDED
#include <map>
#include <string>
#include <vector>
#include "cAudio.h"
#include "../include/IAudioDecoderFactory.h"
#include "cListener.h"
#include "../include/IAudioManager.h"

namespace cAudio
{
	class IAudio;

    class cAudioManager : public IAudioManager
    {
    public:
	//!Inits the audio manager calling the alut/etc start ups
        void init(int argc,char* argv[]); 
	//!Shuts everything down        
	void shutDown();
	//!Creates the cAudio object
        IAudio* createFromFile(const std::string& identifier,const std::string& file,bool stream = false);
	//!Loads ogg from memory or virtual file system
        IAudio* createFromMemory(const std::string& identifier, const char* data, size_t length, std::string ext);
	//!Register Audio Codec        
	void registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension);
	//!Allows you to set the listener position        
	void setListenerPos(float x,float y,float z);
	//!Set Listener Orientation
        void setListenerOrientation(float ux,float uy,float uz);
	//!Updates the cAudio playback        
	void update();
	//!Gets you the cAudio object you want
        IAudio *getSound(std::string identifier);
	//!Releases "ALL" cAudio objects        
	void release();

        static cAudioManager* Instance()
        {
            return &m_cAudioManager;
        }

    protected:
        cAudioManager(){ }

    private:
	//!Global cAudioManager
        static cAudioManager m_cAudioManager;
	//!The map that holds the cAudio objects
        std::map<std::string, IAudio*> audiomap; 
	//!Decoder map that holds all decoders by file extension
        std::map<std::string, IAudioDecoderFactory*> decodermap; 
	//!The listener object        
	cListener initlistener;
	//!The listener X Coordinate
        float listenerX;
	//!The listener Y Coordinate
        float listenerY;
	//!THe listener Z Coordinate
        float listenerZ;
    };
}

#endif //! CAUDIOMANAGER_H_INCLUDED
