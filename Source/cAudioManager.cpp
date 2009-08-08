#include "../Headers/cAudioManager.h"
#include "../Headers/cFileSource.h"
#include "../Headers/cMemorySource.h"
#include "../Headers/cUtils.h"
#include "../Headers/cOggAudioDecoderFactory.h"
#include "../Headers/cWavAudioDecoderFacotry.h"

#include <AL/al.h>
#include <AL/alut.h>
//!#include <AL/alext.h>
//!#include <AL/efx.h>
#include <iostream>
#define LOAD_AL_FUNC(x) (x = (typeof(x))alGetProcAddress(#x))

namespace cAudio
{
    CAUDIO_API IAudioManager* getAudioManager(void)
	{
		return cAudioManager::Instance();
	}
    cAudioManager cAudioManager::m_cAudioManager;

    //!Initialize the listener,openal,and mikmod
    void cAudioManager::init(int argc,char* argv[])
    {
        alutInit(&argc,argv);
		/*
		ALCcontext *Context;
		ALCdevice *Device;
		ALint attribs[4] = {0};
		ALCint iSends = 0;
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;

		Device = alcOpenDevice(NULL);

		//!Query for effect extention
		if(alcIsExtensionPresent(Device,"ALC_EXT_EFX") == AL_FALSE){

		return;
		}

		//!Use context creation hint to request 4 axiliary sends per source
		attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		attribs[1] = 4;
		attribs[2] = 0;

		Context = alcCreateContext(Device,attribs);

		if(!Context)
			return;

		//!activate context

		alcMakeContextCurrent(Context);

		//!retrive Actual number aux sends avaliable per source
		alcGetIntegerv(Device,ALC_MAX_AUXILIARY_SENDS,1,&iSends);

		//!Get the effect extention function pointers
		LOAD_AL_FUNC(alGenEffects);
		LOAD_AL_FUNC(alDeleteEffects);
		LOAD_AL_FUNC(alIsEffect);
		*/

        registerAudioDecoder(new cOggAudioDecoderFactory, "ogg");
        registerAudioDecoder(new cWavAudioDecoderFactory, "wav");
    }

    //!create a sound source
    IAudio* cAudioManager::createFromFile(const std::string& identifier,const std::string& file,bool stream)
    {
        if(stream){
            cFileSource* source = new cFileSource(file);
            if(source->isValid())
            {
                std::string ext = getExt(file);
                std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(ext);
                if(it == decodermap.end())
                {
                    delete source;
                    return NULL;
                }
                IAudioDecoder* decoder = it->second->CreateAudioDecoder(source);
                IAudio* audio = new cAudio(decoder);
                audiomap[identifier] = audio;
                return audio;
            }
            else
            {
                delete source;
                return NULL;
            }
        }
        else
        {
            cFileSource* tempsource = new cFileSource(file);
            int length = tempsource->getSize();
            char *tempbuf = new char[length];
            tempsource->read(tempbuf,length);

            IAudio* guy = createFromMemory(identifier,tempbuf,length,getExt(file));
            delete[]tempbuf;
            delete tempsource;
            return guy;
        }
    }

    //!Loads the ogg file from memory *virtual file systems*
    IAudio* cAudioManager::createFromMemory(const std::string& identifier, const char* data, size_t length, std::string ext)
    {
        cMemorySource* source = new cMemorySource(data,length,true);
        if(source->isValid())
        {
            std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(ext);
            if(it == decodermap.end())
            {
                delete source;
                return NULL;
            }
            IAudioDecoder* decoder = it->second->CreateAudioDecoder(source);
            IAudio* audio = new cAudio(decoder);
            audiomap[identifier] = audio;
            return audio;
        }
        else
        {
            delete source;
            return NULL;
        }
    }

    void cAudioManager::registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension)
    {
        decodermap[extension] = factory;
    }

    //!grabs the selected audio file via the identifier
    IAudio *cAudioManager::getSound(std::string identifier)
    {
        std::map<std::string,IAudio*>::iterator i = audiomap.find(identifier);
        if (i == audiomap.end()){return NULL;}
        return i->second;
    }

    //!Releases the selected audio source
    void cAudioManager::release()
    {
        std::map<std::string,IAudio*>::iterator i = audiomap.begin();
        while ( i != audiomap.end())
        {
            i->second->release();
            delete i->second;
			i++;
        }
		audiomap.clear();
        std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.begin();
        while ( it != decodermap.end())
        {
            delete it->second;
            it++;
        }
		decodermap.clear();
    }

    //!Updates all audiosources created
    void cAudioManager::update()
    {
        std::map<std::string,IAudio*>::iterator i = audiomap.begin();
        for (i = audiomap.begin(); i != audiomap.end() ; i++)
        {
            if (i->second->isvalid() == true)
            {

                if (i->second->update())
                {

                }
                if (i->second->playback())
                {

                }
            }
        }
    }

    //!Shuts down cAudio. Deletes all audio sources in process
    void cAudioManager::shutDown()
    {
        alutExit();
    }

    //!Sets the listeners position.
    void cAudioManager::setListenerPos(float x,float y,float z)
    {
        initlistener.setPosition(x,y,z);
    }

    //!Sets the listener orientation
    void cAudioManager::setListenerOrientation(float ux,float uy,float uz)
    {
		initlistener.setUpVector(ux,uy,uz);
    }

}
