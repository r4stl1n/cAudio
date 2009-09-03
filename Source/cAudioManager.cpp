#include "../Headers/cAudioManager.h"
#include "../Headers/cFileSource.h"
#include "../Headers/cMemorySource.h"
#include "../Headers/cUtils.h"
#include "../Headers/cOggAudioDecoderFactory.h"
#include "../Headers/cWavAudioDecoderFacotry.h"
#include "../Headers/cRawAudioDecoderFactory.h"
#include "../Headers/cThread.h"
#include "../include/cAudioSleep.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/xram.h>

#include <iostream>
#define LOAD_AL_FUNC(x) (x = (typeof(x))alGetProcAddress(#x))

namespace cAudio
{
    CAUDIO_API IAudioManager* getAudioManager(void)
	{
		return cAudioManager::Instance();
	}
    cAudioManager cAudioManager::m_cAudioManager;

	CAUDIO_DECLARE_THREAD_FUNCTION(UpdateThread)
	{
		while(true)
		{
			if(!cAudioManager::Instance()->IsThreadRunning())
			{
				return 0;
			}
			cAudioManager::Instance()->update();
			cAudioSleep(1);
		}
	}

    //!Initialize the listener,openal,and mikmod
    void cAudioManager::init(int argc,char* argv[])
    {
		Mutex.lock();
		//Make a Openal context pointer
		ALCcontext *Context;
		//Make a openal device pointer
		ALCdevice *Device;
		//Create a new device
		Device = alcOpenDevice(NULL);
		//Stores the EAX attributes
		ALint attribs[4] = { 0 };
		//Check if device can be created
		if (Device == NULL)
			exit(-1);

		//Setup attributes to request 4 slots per a sound source
		attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		attribs[1] = 4;

		//Create context with eax effects for windows
#ifdef CAUDIO_EAX_ENABLED
		if(alcIsExtensionPresent(Device, "ALC_EXT_EFX") == AL_FALSE)
			return;

		Context=alcCreateContext(Device, attribs);
#else
		Context=alcCreateContext(Device, NULL);
#endif 

		//Set active context
		alcMakeContextCurrent(Context);
		// Clear Error Code
		alGetError();

		initCapture.checkCaptureExtension();
		initCapture.initialize();

        registerAudioDecoder(new cOggAudioDecoderFactory, "ogg");
        registerAudioDecoder(new cWavAudioDecoderFactory, "wav");
		registerAudioDecoder(new cRawAudioDecoderFactory, "raw");

		Mutex.unlock();

#ifdef CAUDIO_USE_INTERNAL_THREAD
		RunThread = (cAudioThread::SpawnThread(UpdateThread, NULL) == 0);
#endif
    }

    //!create a sound source
    IAudio* cAudioManager::createFromFile(const std::string& identifier,const std::string& file,bool stream)
    {
		Mutex.lock();
        if(stream){
            cFileSource* source = new cFileSource(file);
            if(source->isValid())
            {
                std::string ext = getExt(file);
                IAudioDecoderFactory* factory = getAudioDecoderFactory(ext);
				if(!factory)
				{
                    delete source;
					Mutex.unlock();
                    return NULL;
                }
                IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
                IAudio* audio = new cAudio(decoder);
                audiomap[identifier] = audio;
				Mutex.unlock();
                return audio;
            }
            else
            {
                delete source;
				Mutex.unlock();
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
			Mutex.unlock();
            return guy;
        }
    }

    //!Loads the ogg file from memory *virtual file systems*
    IAudio* cAudioManager::createFromMemory(const std::string& identifier, const char* data, size_t length, std::string ext)
    {
		Mutex.lock();
        cMemorySource* source = new cMemorySource(data,length,true);
        if(source->isValid())
        {
			IAudioDecoderFactory* factory = getAudioDecoderFactory(ext);
            if(!factory)
            {
                delete source;
				Mutex.unlock();
                return NULL;
            }
            IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
            IAudio* audio = new cAudio(decoder);
            audiomap[identifier] = audio;
			Mutex.unlock();
            return audio;
        }
        else
        {
            delete source;
			Mutex.unlock();
            return NULL;
        }
    }

	IAudio* cAudioManager::createFromRaw(const std::string& identifier,const char* data, size_t length, unsigned int frequency, AudioFormats format)
	{
		Mutex.lock();
		cMemorySource* source = new cMemorySource(data,length,true);
        if(source->isValid())
        {
			IAudioDecoderFactory* factory = getAudioDecoderFactory("raw");
            if(!factory)
            {
                delete source;
				Mutex.unlock();
                return NULL;
            }
			IAudioDecoder* decoder = ((cRawAudioDecoderFactory*)factory)->CreateAudioDecoder(source, frequency, format);
            IAudio* audio = new cAudio(decoder);
            audiomap[identifier] = audio;
			Mutex.unlock();
            return audio;
        }
        else
        {
            delete source;
			Mutex.unlock();
            return NULL;
        }
	}

    bool cAudioManager::registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension)
    {
		Mutex.lock();
        decodermap[extension] = factory;
		Mutex.unlock();
		return true;
    }

	void cAudioManager::unRegisterAudioDecoder(std::string extension)
	{
		Mutex.lock();
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(extension);
		if(it != decodermap.end())
		{
			delete it->second;
			decodermap.erase(it);
		}
		Mutex.unlock();
	}

	bool cAudioManager::isAudioDecoderRegistered(std::string extension)
	{
		Mutex.lock();
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(extension);
		bool result = (it != decodermap.end());
		Mutex.unlock();
		return result;
	}

	IAudioDecoderFactory* cAudioManager::getAudioDecoderFactory(std::string extension)
	{
		Mutex.lock();
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(extension);
		if(it != decodermap.end())
		{
			Mutex.unlock();
			return it->second;
		}
		Mutex.unlock();
		return NULL;
	}

    //!grabs the selected audio file via the identifier
    IAudio *cAudioManager::getSound(std::string identifier)
    {
		Mutex.lock();
        std::map<std::string,IAudio*>::iterator i = audiomap.find(identifier);
        if (i == audiomap.end())
		{
			Mutex.unlock();
			return NULL;
		}
		Mutex.unlock();
        return i->second;
    }

    //!Releases the selected audio source
    void cAudioManager::release()
    {
		Mutex.lock();
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
		Mutex.unlock();
    }

    //!Updates all audiosources created
    void cAudioManager::update()
    {
		Mutex.lock();
        std::map<std::string,IAudio*>::iterator i = audiomap.begin();
        for (i = audiomap.begin(); i != audiomap.end() ; i++)
        {
            if (i->second->isvalid() == true)
            {

                if (i->second->update())
                {

                }
                /*if (i->second->playback())
                {

                }*/
            }
        }
		initCapture.updateCaptureBuffer();
		Mutex.unlock();
    }

    //!Shuts down cAudio. Deletes all audio sources in process
    void cAudioManager::shutDown()
    {
		Mutex.lock();
		//Create a openal context pointer
		ALCcontext *Context;
		//Create a openal device pointer
		ALCdevice *Device;

		//Shutdown audio capture
		initCapture.shutdown();

		//Grab current openal context
		Context=alcGetCurrentContext();
		//Grab current device context
		Device=alcGetContextsDevice(Context);
		//Reset context to null
		alcMakeContextCurrent(NULL);
		//Delete the context
		alcDestroyContext(Context);
		//Close the device
		alcCloseDevice(Device);

		Mutex.unlock();
		RunThread = false;
    }

    //!Sets the listeners position.
    void cAudioManager::setListenerPos(float x,float y,float z)
    {
		Mutex.lock();
        initlistener.setPosition(cVector3(x,y,z));
		Mutex.unlock();
    }

    //!Sets the listener orientation
    void cAudioManager::setListenerOrientation(float ux,float uy,float uz)
    {
		Mutex.lock();
		initlistener.setUpVector(cVector3(ux,uy,uz));
		Mutex.unlock();
    }

}
