#include "../Headers/cAudio.h"
#include "../Headers/cLogger.h"
#include "../Headers/cFilter.h"
#include "../Headers/cEffect.h"

namespace cAudio
{
#ifdef CAUDIO_EFX_ENABLED
    cAudio::cAudio(IAudioDecoder* decoder, ALCcontext* context, cEFXFunctions* oALFunctions) 
		: Context(context), Source(0), Decoder(decoder), Loop(false), Valid(false), 
		EFX(oALFunctions), Filter(NULL), EffectSlotsAvailable(0), LastFilterTimeStamp(0)
#else
	cAudio::cAudio(IAudioDecoder* decoder, ALCcontext* context)
		: Context(context), Source(0), Decoder(decoder), Loop(false), Valid(false)
#endif
    {
		cAudioMutexBasicLock lock(Mutex);

		for(int i=0; i<CAUDIO_SOURCE_NUM_BUFFERS; ++i)
			Buffers[i] = 0;

#ifdef CAUDIO_EFX_ENABLED
		for(int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			Effects[i] = NULL;

		for(int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			LastEffectTimeStamp[i] = 0;
#endif

		if(Decoder)
			Decoder->grab();

		//Generates 3 buffers for the ogg file
		alGenBuffers(CAUDIO_SOURCE_NUM_BUFFERS, Buffers);
		bool state = !checkError();
		if(state)
		{
			//Creates one source to be stored.
			alGenSources(1, &Source);
			state = !checkError();
		}
#ifdef CAUDIO_EFX_ENABLED
		Valid = state && (Decoder != NULL) && (Context != NULL) && (EFX != NULL);
#else
		Valid = state && (Decoder != NULL) && (Context != NULL);
#endif

#ifdef CAUDIO_EFX_ENABLED
		int numSlots = 0;
		ALCdevice* device = alcGetContextsDevice(Context);
		alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &numSlots);

		EffectSlotsAvailable = (numSlots <= CAUDIO_SOURCE_MAX_EFFECT_SLOTS) ? numSlots : CAUDIO_SOURCE_MAX_EFFECT_SLOTS;
#endif
    }

    cAudio::~cAudio()
    {
		cAudioMutexBasicLock lock(Mutex);
		if(Decoder)
			Decoder->drop();

#ifdef CAUDIO_EFX_ENABLED
		for(int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
		{
			if(Effects[i])
				Effects[i]->drop();
			Effects[i] = NULL;
		}

		if(Filter)
			Filter->drop();
		Filter = NULL;
#endif
    }

	bool cAudio::play()
	{
		cAudioMutexBasicLock lock(Mutex);
		if (!isPaused()) 
        { 
            int queueSize = 0;
			//Resets the audio to the beginning
			Decoder->setPosition(0, false);
			//Purges all buffers from the source
			alSourcei(Source, AL_BUFFER, 0);
			checkError();
            for(int u = 0; u < CAUDIO_SOURCE_NUM_BUFFERS; u++) 
            { 
                int val = stream(Buffers[u]); 
 
                if(val < 0) 
                {  
                    return false;
                } 
                else if(val > 0) 
                    ++queueSize; 
            } 
            //Stores the sources 3 buffers to be used in the queue 
            alSourceQueueBuffers(Source, queueSize, Buffers); 
			checkError();
        }
#ifdef CAUDIO_EFX_ENABLED
		updateFilter();
		for(unsigned int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			updateEffect(i);
#endif
        alSourcePlay(Source);
		checkError();
		getLogger()->logDebug("Audio Source", "Source playing.");
        return true; 
    }

	bool cAudio::play2d(const bool& toLoop)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcei(Source, AL_SOURCE_RELATIVE, true);
        loop(toLoop);
        bool state = play();
		checkError();
		return state;
    }

	bool cAudio::play3d(const cVector3& position, const float& soundstr, const bool& toLoop)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcei(Source, AL_SOURCE_RELATIVE, false);
        setPosition(position);
        setStrength(soundstr);
        loop(toLoop);
        bool state = play();
		checkError();
		return state;
    }

	void cAudio::pause()
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcePause(Source);
		checkError();
		getLogger()->logDebug("Audio Source", "Source paused.");
    }
     
	void cAudio::stop()
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourceStop(Source);
		checkError();
		getLogger()->logDebug("Audio Source", "Source stopped.");
    }

	void cAudio::loop(const bool& loop)
	{
		cAudioMutexBasicLock lock(Mutex);
        Loop = loop;
    }

	bool cAudio::seek(const float& seconds, bool relative)
	{
		bool state = false;
		cAudioMutexBasicLock lock(Mutex);
        if(Decoder->isSeekingSupported())
        {
			state = Decoder->seek(seconds, relative);
        }
		return state;
    }

	bool cAudio::update()
	{
		cAudioMutexBasicLock lock(Mutex);
        if(!isValid() || !isPlaying())
		{
            return false;
		}
        int processed = 0;
        bool active = true;

#ifdef CAUDIO_EFX_ENABLED
		updateFilter();
		for(unsigned int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			updateEffect(i);
#endif

		//gets the sound source processed buffers
        alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
		//while there is more data refill buffers with audio data.
		while (processed--)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(Source, 1, &buffer);
            active = stream(buffer);

			//if more in stream continue playing.
            if(active)
                alSourceQueueBuffers(Source, 1, &buffer);

			checkError();
        }
		return active;
    }

	void cAudio::release()
    {
		cAudioMutexBasicLock lock(Mutex);
		//Stops the audio Source
		alSourceStop(Source);
		empty();
		//Deletes the source
		alDeleteSources(1, &Source);
		//deletes the last filled buffer
		alDeleteBuffers(CAUDIO_SOURCE_NUM_BUFFERS, Buffers);
		checkError();
		getLogger()->logDebug("Audio Source", "Audio source released.");
    }

	const bool cAudio::isValid() const
	{
        return Valid;
	}

	const bool cAudio::isPlaying() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

	const bool cAudio::isPaused() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        return (state == AL_PAUSED);
    }

	const bool cAudio::isStopped() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
		return (state == AL_STOPPED);
    }

	const bool cAudio::isLooping() const
	{
		return Loop;
	}
     
	void cAudio::setPosition(const cVector3& position)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkError();
    }

	void cAudio::setVelocity(const cVector3& velocity)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		checkError();
    }

	void cAudio::setDirection(const cVector3& direction)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_DIRECTION, direction.x, direction.y, direction.z);
		checkError();
    }

	void cAudio::setRolloffFactor(const float& rolloff)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_ROLLOFF_FACTOR, rolloff);
		checkError();
    }

	void cAudio::setStrength(const float& soundstrength)
	{
		float inverseStrength = 0.0f;
		if(soundstrength > 0.0f)
			inverseStrength = 1.0f / soundstrength;

		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_ROLLOFF_FACTOR, inverseStrength);
		checkError();
    }

	void cAudio::setMinDistance(const float& minDistance)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_REFERENCE_DISTANCE, minDistance);
		checkError();
	}

	void cAudio::setMaxDistance(const float& maxDistance)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MAX_DISTANCE, maxDistance);
		checkError();
	}

	void cAudio::setPitch(const float& pitch)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef (Source, AL_PITCH, pitch);
		checkError();
    }

	void cAudio::setVolume(const float& volume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_GAIN, volume);
		checkError();
    }

	void cAudio::setMinVolume(const float& minVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MIN_GAIN, minVolume);
		checkError();
	}

	void cAudio::setMaxVolume(const float& maxVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MAX_GAIN, maxVolume);
		checkError();
	}

	void cAudio::setInnerConeAngle(const float& innerAngle)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_INNER_ANGLE, innerAngle);
		checkError();
	}

	void cAudio::setOuterConeAngle(const float& outerAngle)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_OUTER_ANGLE, outerAngle);
		checkError();
	}

	void cAudio::setOuterConeVolume(const float& outerVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_OUTER_GAIN, outerVolume);
		checkError();
	}

	void cAudio::setDopplerStrength(const float& dstrength)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_DOPPLER_FACTOR, dstrength);
		checkError();
    }

	void cAudio::setDopplerVelocity(const cVector3& dvelocity)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_DOPPLER_VELOCITY, dvelocity.x, dvelocity.y, dvelocity.z);
		checkError();
    }

	void cAudio::move(const cVector3& position)
	{
		cAudioMutexBasicLock lock(Mutex);
		cVector3 oldPos = getPosition();
		cVector3 velocity = position - oldPos;

        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkError();
	}

	const cVector3 cAudio::getPosition() const
	{
		cVector3 position;
		alGetSourcefv(Source, AL_POSITION, &position.x);
		return position;
	}

	const cVector3 cAudio::getVelocity() const
	{
		cVector3 velocity;
		alGetSourcefv(Source, AL_VELOCITY, &velocity.x);
		return velocity;
	}

	const cVector3 cAudio::getDirection() const
	{
		cVector3 direction;
		alGetSourcefv(Source, AL_DIRECTION, &direction.x);
		return direction;
	}

	const float cAudio::getRolloffFactor() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_ROLLOFF_FACTOR, &value);
		return value;
	}

	const float cAudio::getStrength() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_ROLLOFF_FACTOR, &value);

		float inverseStrength = 0.0f;
		if(value > 0.0f)
			inverseStrength = 1.0f / value;

		return inverseStrength;
	}

	const float cAudio::getMinDistance() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_REFERENCE_DISTANCE, &value);
		return value;
	}

	const float cAudio::getMaxDistance() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MAX_DISTANCE, &value);
		return value;
	}

	const float cAudio::getPitch() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_PITCH, &value);
		return value;
	}

	const float cAudio::getVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_GAIN, &value);
		return value;
	}

	const float cAudio::getMinVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MIN_GAIN, &value);
		return value;
	}

	const float cAudio::getMaxVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MAX_GAIN, &value);
		return value;
	}

	const float cAudio::getInnerConeAngle() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_INNER_ANGLE, &value);
		return value;
	}

	const float cAudio::getOuterConeAngle() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_OUTER_ANGLE, &value);
		return value;
	}

	const float cAudio::getOuterConeVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_OUTER_GAIN, &value);
		return value;
	}

	const float cAudio::getDopplerStrength() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_DOPPLER_FACTOR, &value);
		return value;
	}

	const cVector3 cAudio::getDopplerVelocity() const
	{
		cVector3 velocity;
		alGetSourcefv(Source, AL_DOPPLER_VELOCITY, &velocity.x);
		return velocity;
	}

#ifdef CAUDIO_EFX_ENABLED
	unsigned int cAudio::getNumEffectSlotsAvailable() const
	{
		return EffectSlotsAvailable;
	}

	bool cAudio::attachEffect(unsigned int slot, IEffect* effect)
	{
		cAudioMutexBasicLock lock(Mutex);
		if(slot < EffectSlotsAvailable)
		{
			Effects[slot] = effect;

			if(Effects[slot])
				Effects[slot]->grab();

			updateEffect(slot);
			return true;
		}
		return false;
	}

	void cAudio::removeEffect(unsigned int slot)
	{
		cAudioMutexBasicLock lock(Mutex);
		if(slot < EffectSlotsAvailable)
		{
			if(Effects[slot])
				Effects[slot]->drop();

			Effects[slot] = NULL;
			LastEffectTimeStamp[slot] = 0;
			updateEffect(slot, true);
		}
	}

	bool cAudio::attachFilter(IFilter* filter)
	{
		cAudioMutexBasicLock lock(Mutex);
		Filter = filter;

		if(Filter)
			Filter->grab();

		updateFilter();
		return true;
	}

	void cAudio::removeFilter()
	{
		cAudioMutexBasicLock lock(Mutex);
		if(Filter)
			Filter->drop();
		Filter = NULL;
		LastFilterTimeStamp = 0;
		updateFilter(true);
	}
#endif

    void cAudio::empty()
    {
        int queued = 0;
        alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);

        while (queued--)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(Source, 1, &buffer);
			checkError();
        }
    }

	bool cAudio::checkError()
    {
        int error = alGetError();
		const char* errorString;

        if (error != AL_NO_ERROR)
        {
			errorString = alGetString(error);
			if(error == AL_OUT_OF_MEMORY)
				getLogger()->logCritical("Audio Source", "OpenAL Error: %s.", errorString);
			else
				getLogger()->logError("Audio Source", "OpenAL Error: %s.", errorString);
			return true;
        }
		return false;
    }

    bool cAudio::stream(ALuint buffer)
    {
        if(Decoder)
        {
	        //stores the caculated data into buffer that is passed to output.
			size_t totalread = 0;
			unsigned int errorcount = 0;
	        char tempbuffer[CAUDIO_SOURCE_BUFFER_SIZE];
			while( totalread < CAUDIO_SOURCE_BUFFER_SIZE )
			{
				char tempbuffer2[CAUDIO_SOURCE_BUFFER_SIZE];
				int actualread = Decoder->readAudioData(tempbuffer2, CAUDIO_SOURCE_BUFFER_SIZE-totalread);
				if(actualread > 0)
				{
					memcpy(tempbuffer+totalread,tempbuffer2,actualread);
					totalread += actualread;
				}
				if(actualread < 0)
				{
					++errorcount;
					getLogger()->logDebug("Audio Source", "Decoder returned an error: %i (%i of 3)", actualread, errorcount);
					if(errorcount >= 3)
						break;
				}
				if(actualread == 0)
				{
					if(isLooping())
					{
						//If we are to loop, set to the beginning and reload from the start
						Decoder->setPosition(0, false);
						getLogger()->logDebug("Audio Source", "Buffer looping.");
					}
					else
						break;
				}
			}

	        //Second check, in case looping is not enabled, we will return false for end of stream
	        if(totalread == 0)
	       	{
	       		 return false;
	        }
			getLogger()->logDebug("Audio Source", "Buffered %i bytes of data into buffer %i at %i hz.", totalread, buffer, Decoder->getFrequency());
            alBufferData(buffer, convertAudioFormatEnum(Decoder->getFormat()), tempbuffer, totalread, Decoder->getFrequency());
			checkError();
            return true;
        }
		return false;
    }

	ALenum cAudio::convertAudioFormatEnum(AudioFormats format)
	{
		switch(format)
		{
		case EAF_8BIT_MONO:
			return AL_FORMAT_MONO8;
		case EAF_16BIT_MONO:
			return AL_FORMAT_MONO16;
		case EAF_8BIT_STEREO:
			return AL_FORMAT_STEREO8;
		case EAF_16BIT_STEREO:
			return AL_FORMAT_STEREO16;
		default:
			return AL_FORMAT_MONO8;
		};
	}

#ifdef CAUDIO_EFX_ENABLED
	void cAudio::updateFilter(bool remove)
	{
		if(!remove)
		{
			if(Filter && Filter->isValid())
			{
				if(LastFilterTimeStamp != Filter->getLastUpdated())
				{
					LastFilterTimeStamp = Filter->getLastUpdated();
					cFilter* theFilter = static_cast<cFilter*>(Filter);
					if(theFilter)
					{
						alSourcei(Source, AL_DIRECT_FILTER, theFilter->getOpenALFilter());
						checkError();
						return;
					}
				}
				return;
			}
		}
		alSourcei(Source, AL_DIRECT_FILTER, AL_FILTER_NULL);
		checkError();
	}

	void cAudio::updateEffect(unsigned int slot, bool remove)
	{
		if(slot < EffectSlotsAvailable)
		{
			if(!remove)
			{
				if(Effects[slot] && Effects[slot]->isValid())
				{
					if(LastEffectTimeStamp[slot] != Effects[slot]->getLastUpdated())
					{
						LastEffectTimeStamp[slot] = Effects[slot]->getLastUpdated();
						cEffect* theEffect = static_cast<cEffect*>(Effects[slot]);
						if(theEffect)
						{
							ALuint filterID = AL_FILTER_NULL;
							cFilter* theFilter = static_cast<cFilter*>(theEffect->getFilter());
							if(theFilter)
							{
								filterID = theFilter->getOpenALFilter();
							}
							alSource3i(Source, AL_AUXILIARY_SEND_FILTER, theEffect->getOpenALEffectSlot(), slot, filterID);
							checkError();
							return;
						}
					}
					return;
				}
			}
			alSource3i(Source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, slot, AL_FILTER_NULL);
			checkError();
		}
	}
#endif
}
