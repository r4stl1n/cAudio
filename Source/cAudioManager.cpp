#include "../Headers/cAudioManager.h"
#include "../Headers/cFileSource.h"
#include "../Headers/cMemorySource.h"
#include "../Headers/cUtils.h"
#include "../Headers/cOggAudioDecoderFactory.h"
#include "../Headers/cWavAudioDecoderFacotry.h"
#include "../Headers/cRawAudioDecoderFactory.h"
#include "../Headers/cThread.h"
#include "../include/cAudioSleep.h"
#include "../Headers/cLogger.h"

#include <set>

#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/xram.h>

#define LOAD_AL_FUNC(x) (x = (typeof(x))alGetProcAddress(#x))

namespace cAudio
{
	static bool RunAudioManagerThread(false);

#ifdef CAUDIO_COMPILE_WITH_OGG_DECODER
	static cOggAudioDecoderFactory OggDecoderFactory;
#endif
#ifdef CAUDIO_COMPILE_WITH_WAV_DECODER
	static cWavAudioDecoderFactory WavDecoderFactory;
#endif
#ifdef CAUDIO_COMPILE_WITH_RAW_DECODER
	static cRawAudioDecoderFactory RawDecoderFactory;
#endif

	//Note: OpenAL is threadsafe, so a mutex only needs to protect the class state
#ifdef CAUDIO_USE_INTERNAL_THREAD
	static cAudioMutex AudioManagerObjectsMutex;
	static std::set<IAudioManager*> AudioManagerObjects;

	CAUDIO_DECLARE_THREAD_FUNCTION(AudioManagerUpdateThread)
	{
		while(RunAudioManagerThread)
		{
			AudioManagerObjectsMutex.lock();
			std::set<IAudioManager*>::iterator it;
			for ( it=AudioManagerObjects.begin() ; it != AudioManagerObjects.end(); it++ )
			{
				(*it)->update();
			}
			AudioManagerObjectsMutex.unlock();
			cAudioSleep(1);
		}
		return 0;
	}
#endif

    //!Initialize the listener,openal,and mikmod
    bool cAudioManager::initialize(const char* deviceName, int outputFrequency, int eaxEffectSlots)
    {
		Mutex.lock();

		//Stores the context attributes (MAX of 4, with 2 zeros to terminate)
		ALint attribs[6] = { 0 };

		unsigned int currentAttrib = 0;
		if(outputFrequency > 0)
		{
			attribs[currentAttrib++] = ALC_FREQUENCY;
			attribs[currentAttrib++] = outputFrequency;
		}
		if(eaxEffectSlots > 0)
		{
			attribs[currentAttrib++] = ALC_MAX_AUXILIARY_SENDS;
			attribs[currentAttrib++] = eaxEffectSlots;
		}

		//Create a new device
		Device = alcOpenDevice(deviceName);
		//Check if device can be created
		if (Device == NULL)
		{
			getLogger()->logError("AudioManager", "Failed to Create OpenAL Device.");
			checkError();
			Mutex.unlock();
			return false;
		}

		//Create context with eax effects for windows
#ifdef CAUDIO_EAX_ENABLED
		if(alcIsExtensionPresent(Device, "ALC_EXT_EFX") == AL_FALSE)
		{
			getLogger()->logWarning("AudioManager", "EFX not supported.");
		}
		getLogger()->logInfo("AudioManager", "EFX supported and enabled.");
#endif

		Context = alcCreateContext(Device, attribs);
		if (Context == NULL)
		{
			getLogger()->logError("AudioManager", "Failed to Create OpenAL Context.");
			checkError();
			Mutex.unlock();
			return false;
		}

		if(!alcMakeContextCurrent(Context))
		{
			getLogger()->logError("AudioManager", "Failed to make OpenAL Context current.");
			checkError();
			Mutex.unlock();
			return false;
		}

		getLogger()->logInfo("AudioManager", "OpenAL Version: %s", alGetString(AL_VERSION));
		getLogger()->logInfo("AudioManager", "Vendor: %s", alGetString(AL_VENDOR));
		getLogger()->logInfo("AudioManager", "Renderer: %s", alGetString(AL_RENDERER));
		getLogger()->logInfo("AudioManager", "Supported Extensions: %s", alGetString(AL_EXTENSIONS));

#ifdef CAUDIO_COMPILE_WITH_OGG_DECODER
        registerAudioDecoder(&OggDecoderFactory, "ogg");
#endif
#ifdef CAUDIO_COMPILE_WITH_WAV_DECODER
        registerAudioDecoder(&WavDecoderFactory, "wav");
#endif
#ifdef CAUDIO_COMPILE_WITH_RAW_DECODER
		registerAudioDecoder(&RawDecoderFactory, "raw");
#endif

		Mutex.unlock();
		return true;
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
				getLogger()->logInfo("AudioManager", "Streaming Audio Source (%s) created from file %s.", identifier.c_str(), file.c_str());
                Mutex.unlock();
				return audio;
            }
            else
            {
                delete source;
				getLogger()->logError("AudioManager", "Failed to create Audio Source (%s) from file %s.", identifier.c_str(), file.c_str());
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
			getLogger()->logInfo("AudioManager", "Audio Source (%s) created from memory buffer.", identifier.c_str());
			Mutex.unlock();
            return audio;
        }
        else
        {
            delete source;
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s) from memory buffer.", identifier.c_str());
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
			getLogger()->logInfo("AudioManager", "Raw Audio Source (%s) created from memory buffer.", identifier.c_str());
			Mutex.unlock();
            return audio;
        }
        else
        {
            delete source;
			getLogger()->logError("AudioManager", "Failed to create Raw Audio Source (%s) from memory buffer.", identifier.c_str());
			Mutex.unlock();
            return NULL;
        }
	}

    bool cAudioManager::registerAudioDecoder(IAudioDecoderFactory* factory, std::string extension)
    {
		Mutex.lock();
        decodermap[extension] = factory;
		getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s registered.", extension.c_str());
		Mutex.unlock();
		return true;
    }

	void cAudioManager::unRegisterAudioDecoder(std::string extension)
	{
		Mutex.lock();
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(extension);
		if(it != decodermap.end())
		{
			decodermap.erase(it);
			getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s unregistered.", extension.c_str());
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
            if (i->second->isValid() == true)
            {
                if (i->second->update())
                {

                }
            }
        }
		Mutex.unlock();
    }

    //!Shuts down cAudio. Deletes all audio sources in process
    void cAudioManager::shutDown()
    {
		Mutex.lock();
		//Reset context to null
		alcMakeContextCurrent(NULL);
		//Delete the context
		alcDestroyContext(Context);
		//Close the device
		alcCloseDevice(Device);
		getLogger()->logInfo("AudioManager", "Manager successfully shutdown.");
		Mutex.unlock();
    }

	void cAudioManager::checkError()
	{
		int error = alGetError();
		const char* errorString;

        if (error != AL_NO_ERROR)
        {
			errorString = alGetString(error);
			getLogger()->logError("AudioManager", "OpenAL Error: %s.", errorString);
        }

		if(Device)
		{
			error = alcGetError(Device);
			if (error != AL_NO_ERROR)
			{
				errorString = alGetString(error);
				getLogger()->logError("AudioManager", "OpenAL Error: %s.", errorString);
			}
		}
	}

	void cAudioManager::getAvailableDevices()
	{
		// Get list of available Playback Devices
		Mutex.lock();
		if( alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE )
		{
			const char* deviceList = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
			if (deviceList)
			{
				while(*deviceList)
				{
					std::string device(deviceList);
					AvailableDevices.push_back(device);
					deviceList += strlen(deviceList) + 1;
				}
			}

			// Get the name of the 'default' capture device
			DefaultDevice = alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
		}
		else if( alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE )
		{
			const char* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
			if (deviceList)
			{
				while(*deviceList)
				{
					std::string device(deviceList);
					AvailableDevices.push_back(device);
					deviceList += strlen(deviceList) + 1;
				}
			}

			// Get the name of the 'default' capture device
			DefaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		}
		Mutex.unlock();
	}

	const char* cAudioManager::getAvailableDeviceName(unsigned int index)
	{
		Mutex.lock();
		if(!AvailableDevices.empty())
		{
			//Bounds check
			if( index > (AvailableDevices.size()-1) ) index = (AvailableDevices.size()-1);
			const char* deviceName = AvailableDevices[index].c_str();
			Mutex.unlock();
			return deviceName;
		}
		Mutex.unlock();
		return "";
	}

	unsigned int cAudioManager::getAvailableDeviceCount()
	{
		Mutex.lock();
		unsigned int size = AvailableDevices.size();
		Mutex.unlock();
		return size;
	}

	const char* cAudioManager::getDefaultDeviceName()
	{
		Mutex.lock();
		const char* deviceName = DefaultDevice.empty() ? "" : DefaultDevice.c_str();
		Mutex.unlock();
		return deviceName;
	}

	CAUDIO_API IAudioManager* createAudioManager(bool initializeDefault)
	{
		cAudioManager* manager = new cAudioManager;
		if(manager)
		{
			if(initializeDefault)
				manager->initialize();

			manager->getAvailableDevices();

#ifdef CAUDIO_USE_INTERNAL_THREAD
			AudioManagerObjectsMutex.lock();
			AudioManagerObjects.insert(manager);

			//First time launch of thread
			if(!RunAudioManagerThread && AudioManagerObjects.size() > 0)
				RunAudioManagerThread = (cAudioThread::SpawnThread(AudioManagerUpdateThread, NULL) == 0);
			AudioManagerObjectsMutex.unlock();
#endif
		}
		return manager;
	}

	CAUDIO_API void destroyAudioManager(IAudioManager* manager)
	{
		if(manager)
		{
#ifdef CAUDIO_USE_INTERNAL_THREAD
			AudioManagerObjectsMutex.lock();
			AudioManagerObjects.erase(manager);

			//Kill the thread if there are no objects to process anymore
			if(RunAudioManagerThread && AudioManagerObjects.empty())
				RunAudioManagerThread = false;
			AudioManagerObjectsMutex.unlock();
#endif
			delete manager;
			manager = NULL;
		}
	}

	CAUDIO_API bool isAudioManagerThreadRunning()
	{
		return RunAudioManagerThread;
	}

};
