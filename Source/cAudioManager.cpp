#include "../Headers/cAudioManager.h"
#include "../Headers/cFileSource.h"
#include "../Headers/cMemorySource.h"
#include "../Headers/cUtils.h"
#include "../Headers/cOggAudioDecoderFactory.h"
#include "../Headers/cWavAudioDecoderFactory.h"
#include "../Headers/cRawAudioDecoderFactory.h"
#include "../Headers/cThread.h"
#include "../include/cAudioSleep.h"
#include "../Headers/cLogger.h"

#include <set>

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
		cAudioMutexBasicLock lock(Mutex);

		if(Initialized)
			return false;

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
			return false;
		}

		Context = alcCreateContext(Device, attribs);
		if (Context == NULL)
		{
			getLogger()->logError("AudioManager", "Failed to Create OpenAL Context.");
			checkError();
			alcCloseDevice(Device);
			Device = NULL;
			return false;
		}

		if(!alcMakeContextCurrent(Context))
		{
			getLogger()->logError("AudioManager", "Failed to make OpenAL Context current.");
			checkError();
			alcDestroyContext(Context);
			alcCloseDevice(Device);
			Context = NULL;
			Device = NULL;
			return false;
		}

#ifdef CAUDIO_EFX_ENABLED
		initEffects.getEFXInterface()->Mutex.lock();
		EFXSupported = initEffects.getEFXInterface()->CheckEFXSupport(Device);
		initEffects.getEFXInterface()->Mutex.unlock();
		initEffects.checkEFXSupportDetails();
#endif

		getLogger()->logInfo("AudioManager", "OpenAL Version: %s", alGetString(AL_VERSION));
		getLogger()->logInfo("AudioManager", "Vendor: %s", alGetString(AL_VENDOR));
		getLogger()->logInfo("AudioManager", "Renderer: %s", alGetString(AL_RENDERER));
#ifdef CAUDIO_EFX_ENABLED
		if(EFXSupported)
		{
			int EFXMajorVersion = 0;
			int EFXMinorVersion = 0;
			alcGetIntegerv(Device, ALC_EFX_MAJOR_VERSION, 1, &EFXMajorVersion);
			alcGetIntegerv(Device, ALC_EFX_MINOR_VERSION, 1, &EFXMinorVersion);
			getLogger()->logInfo("AudioManager", "EFX Version: %i.%i", EFXMajorVersion, EFXMinorVersion);
			getLogger()->logInfo("AudioManager", "EFX supported and enabled.");
		}
		else
		{
			getLogger()->logWarning("AudioManager", "EFX is not supported, EFX disabled.");
		}
#endif
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

		Initialized = true;
		return true;
    }

    //!create a sound source
    IAudio* cAudioManager::createFromFile(const char* name, const char* pathToFile, bool stream)
    {
		cAudioMutexBasicLock lock(Mutex);

		std::string audioName = safeCStr(name);
		std::string path = safeCStr(pathToFile);

		std::string ext = getExt(path);
		IAudioDecoderFactory* factory = getAudioDecoderFactory(ext.c_str());
		if(factory)
		{
			if(stream)
			{
				cFileSource* source = new cFileSource(path);
				if(source)
				{
					if(source->isValid())
					{
						IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
						source->drop();
						if(decoder)
						{
							if(decoder->isValid())
							{
								IAudio* audio = new cAudio(decoder, Context, initEffects.getEFXInterface());
								decoder->drop();

								if(audio)
								{
									if(audio->isValid())
									{
										if(!audioName.empty())
											audioIndex[audioName] = audio;

										audioSources.push_back(audio);

										getLogger()->logInfo("AudioManager", "Streaming Audio Source (%s) created from file %s.", audioName.c_str(), path.c_str());
										
										return audio;
									}
									getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Error creating audio source.", audioName.c_str());
									audio->drop();
									return NULL;
								}
								getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
								return NULL;
							}
							getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data could not be decoded by (.%s) decoder.", audioName.c_str(), ext.c_str());
							decoder->drop();
							return NULL;
						}
						getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
						return NULL;
					}
					source->drop();
					getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not load file %s.", audioName.c_str(), path.c_str());
					return NULL;
				}
				getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
				return NULL;
			}
			else
			{
				cFileSource* tempsource = new cFileSource(path);
				if(tempsource)
				{
					if(tempsource->isValid())
					{
						int length = tempsource->getSize();
						char *tempbuf = new char[length];
						if(tempbuf)
						{
							tempsource->read(tempbuf,length);

							IAudio* guy = createFromMemory(name, tempbuf, length, getExt(path).c_str());
							delete[]tempbuf;
							tempsource->drop();
							return guy;
						}
						getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
						tempsource->drop();
						return NULL;
					}
					getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data is corrupt.", audioName.c_str());
					tempsource->drop();
					return NULL;
				}
				getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
				return NULL;
			}
		}
		getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Codec (.%s) is not supported.", audioName.c_str(), ext.c_str());
		return NULL;
    }

    //!Loads the ogg file from memory *virtual file systems*
    IAudio* cAudioManager::createFromMemory(const char* name, const char* data, size_t length, const char* extension)
    {
		cAudioMutexBasicLock lock(Mutex);

		std::string audioName = safeCStr(name);
		std::string ext = safeCStr(extension);
		IAudioDecoderFactory* factory = getAudioDecoderFactory(ext.c_str());
		if(factory)
		{
			cMemorySource* source = new cMemorySource(data, length, true);
			if(source)
			{
				if(source->isValid())
				{
					IAudioDecoder* decoder = factory->CreateAudioDecoder(source);
					source->drop();
					if(decoder)
					{
						if(decoder->isValid())
						{
							IAudio* audio = new cAudio(decoder, Context, initEffects.getEFXInterface());
							decoder->drop();

							if(audio)
							{
								if(audio->isValid())
								{
									if(!audioName.empty())
										audioIndex[audioName] = audio;

									audioSources.push_back(audio);

									getLogger()->logInfo("AudioManager", "Audio Source (%s) successfully created from memory.", audioName.c_str());
									
									return audio;
								}
								audio->drop();
								getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Error creating audio source.", audioName.c_str());
								return NULL;
							}
							getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
							return NULL;
						}
						decoder->drop();
						getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data could not be decoded by (.%s) decoder.", audioName.c_str(), ext.c_str());
						return NULL;
					}
					getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory for decoder.", audioName.c_str());
					return NULL;
				}
				source->drop();
				getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data is corrupt.", audioName.c_str());
				return NULL;
			}
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
			return NULL;
		}
		getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Codec (.%s) is not supported.", audioName.c_str(), ext.c_str());
		return NULL;
    }

	IAudio* cAudioManager::createFromRaw(const char* name, const char* data, size_t length, unsigned int frequency, AudioFormats format)
	{
		cAudioMutexBasicLock lock(Mutex);

		std::string audioName = safeCStr(name);
		IAudioDecoderFactory* factory = getAudioDecoderFactory("raw");
		if(factory)
		{
			cMemorySource* source = new cMemorySource(data, length, true);
			if(source)
			{
				if(source->isValid())
				{
					IAudioDecoder* decoder = ((cRawAudioDecoderFactory*)factory)->CreateAudioDecoder(source, frequency, format);
					source->drop();
					if(decoder)
					{
						if(decoder->isValid())
						{
							IAudio* audio = new cAudio(decoder, Context, initEffects.getEFXInterface());
							decoder->drop();

							if(audio)
							{
								if(audio->isValid())
								{
									if(!audioName.empty())
										audioIndex[audioName] = audio;

									audioSources.push_back(audio);

									getLogger()->logInfo("AudioManager", "Audio Source (%s) successfully created from raw data.", audioName.c_str());
									
									return audio;
								}
								audio->drop();
								getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Error creating audio source.", audioName.c_str());
								return NULL;
							}
							getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
							return NULL;
						}
						decoder->drop();
						getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data could not be decoded by (.%s) decoder.", audioName.c_str(), "raw");
						return NULL;
					}
					getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory for decoder.", audioName.c_str());
					return NULL;
				}
				source->drop();
				getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Audio data is corrupt.", audioName.c_str());
				return NULL;
			}
			getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Could not allocate enough memory.", audioName.c_str());
			return NULL;
		}
		getLogger()->logError("AudioManager", "Failed to create Audio Source (%s): Codec (.%s) is not supported.", audioName.c_str(), "raw");
		return NULL;
	}

    bool cAudioManager::registerAudioDecoder(IAudioDecoderFactory* factory, const char* extension)
    {
		cAudioMutexBasicLock lock(Mutex);
		std::string ext = safeCStr(extension);
        decodermap[ext] = factory;
		getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s registered.", ext.c_str());
		return true;
    }

	void cAudioManager::unRegisterAudioDecoder(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		std::string ext = safeCStr(extension);
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(ext);
		if(it != decodermap.end())
		{
			decodermap.erase(it);
			getLogger()->logInfo("AudioManager", "Audio Decoder for extension .%s unregistered.", ext.c_str());
		}
	}

	bool cAudioManager::isAudioDecoderRegistered(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		std::string ext = safeCStr(extension);
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(ext);
		return (it != decodermap.end());
	}

	IAudioDecoderFactory* cAudioManager::getAudioDecoderFactory(const char* extension)
	{
		cAudioMutexBasicLock lock(Mutex);
		std::string ext = safeCStr(extension);
		std::map<std::string, IAudioDecoderFactory*>::iterator it = decodermap.find(ext);
		if(it != decodermap.end())
		{
			return it->second;
		}
		return NULL;
	}

    //!grabs the selected audio file via the identifier
    IAudio* cAudioManager::getSoundByName(const char* name)
    {
		cAudioMutexBasicLock lock(Mutex);
		std::string audioName = safeCStr(name);
        std::map<std::string,IAudio*>::iterator i = audioIndex.find(audioName);
        if (i == audioIndex.end())
		{
			return NULL;
		}
        return i->second;
    }

    //!Releases the selected audio source
    void cAudioManager::release()
    {
		cAudioMutexBasicLock lock(Mutex);
		for(unsigned int i=0; i<audioSources.size(); ++i)
		{
			IAudio* source = audioSources[i];
			if(source)
				source->drop();
		}
		audioSources.clear();
		audioIndex.clear();
    }

    //!Updates all audiosources created
    void cAudioManager::update()
    {
		cAudioMutexBasicLock lock(Mutex);
        for(unsigned int i=0; i<audioSources.size(); ++i)
		{
			IAudio* source = audioSources[i];
            if (source->isValid())
            {
                if (source->update())
                {

                }
            }
        }
    }

    //!Shuts down cAudio. Deletes all audio sources in process
    void cAudioManager::shutDown()
    {
		if(Initialized)
		{
			cAudioMutexBasicLock lock(Mutex);
			release();
			decodermap.clear();
			//Reset context to null
			alcMakeContextCurrent(NULL);
			//Delete the context
			alcDestroyContext(Context);
			Context = NULL;
			//Close the device
			alcCloseDevice(Device);
			Device = NULL;
			Initialized = false;
			getLogger()->logInfo("AudioManager", "Manager successfully shutdown.");
		}
    }

	bool cAudioManager::checkError()
	{
		int error = alGetError();
		const char* errorString;

        if (error != AL_NO_ERROR)
        {
			errorString = alGetString(error);
			getLogger()->logError("AudioManager", "OpenAL Error: %s.", errorString);
			return true;
        }

		if(Device)
		{
			error = alcGetError(Device);
			if (error != AL_NO_ERROR)
			{
				errorString = alGetString(error);
				getLogger()->logError("AudioManager", "OpenAL Error: %s.", errorString);
				return true;
			}
		}
		return false;
	}

	void cAudioManager::getAvailableDevices()
	{
		// Get list of available Playback Devices
		cAudioMutexBasicLock lock(Mutex);
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
	}

	const char* cAudioManager::getAvailableDeviceName(unsigned int index)
	{
		cAudioMutexBasicLock lock(Mutex);
		if(!AvailableDevices.empty())
		{
			//Bounds check
			if( index > (AvailableDevices.size()-1) ) index = (AvailableDevices.size()-1);
			const char* deviceName = AvailableDevices[index].c_str();
			return deviceName;
		}
		return "";
	}

	unsigned int cAudioManager::getAvailableDeviceCount()
	{
		cAudioMutexBasicLock lock(Mutex);
		return AvailableDevices.size();
	}

	const char* cAudioManager::getDefaultDeviceName()
	{
		cAudioMutexBasicLock lock(Mutex);
		return DefaultDevice.empty() ? "" : DefaultDevice.c_str();
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
			manager->shutDown();
			delete manager;
			manager = NULL;
		}
	}

	CAUDIO_API bool isAudioManagerThreadRunning()
	{
		return RunAudioManagerThread;
	}

};
