#include "../Headers/cAudio.h"
#include "../Headers/cLogger.h"

namespace cAudio
{
    cAudio::cAudio(IAudioDecoder* decoder) : Decoder(decoder)
    {
		Mutex.lock();
		Loop = false;

		//Generates 3 buffers for the ogg file
		alGenBuffers(NUM_BUFFERS, Buffers);
		//Creates one source to be stored.
		alGenSources(1, &Source);
		checkError();
		Mutex.unlock();
    }

    cAudio::~cAudio()
    {
		Mutex.lock();
        delete Decoder;
		Mutex.unlock();
    }

	bool cAudio::play()
	{
		Mutex.lock();
		if (!isPaused()) 
        { 
            int queueSize = 0; 
            for(int u = 0; u < NUM_BUFFERS; u++) 
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
        alSourcePlay(Source);
		checkError();
		Mutex.unlock();
		getLogger()->logDebug("Audio Source", "Source playing.");
        return true; 
    }

	bool cAudio::play2d(const bool& toLoop)
	{
		Mutex.lock();
        alSourcei(Source, AL_SOURCE_RELATIVE, true);
        loop(toLoop);
        bool state = play();
		checkError();
		Mutex.unlock();
		return state;
    }

	bool cAudio::play3d(const cVector3& position, const float& soundstr, const bool& toLoop)
	{
		Mutex.lock();
        alSourcei(Source, AL_SOURCE_RELATIVE, false);
        setPosition(position);
        setStrength(soundstr);
        loop(toLoop);
        bool state = play();
		checkError();
		Mutex.unlock();
		return state;
    }

	void cAudio::pause()
	{
		Mutex.lock();
        alSourcePause(Source);
		checkError();
		Mutex.unlock();
		getLogger()->logDebug("Audio Source", "Source paused.");
    }
     
	void cAudio::stop()
	{
		Mutex.lock();
        alSourceStop(Source);
		checkError();
		Mutex.unlock();
		getLogger()->logDebug("Audio Source", "Source stopped.");
    }

	void cAudio::loop(const bool& loop)
	{
		Mutex.lock();
        Loop = loop;
		Mutex.unlock();
    }

	bool cAudio::seek(const float& seconds)
	{
		bool state = false;
		Mutex.lock();
        if(Decoder->isSeekingSupported())
        {
			state = Decoder->seek(seconds, false);
        }
		Mutex.unlock();
		return state;
    }

	bool cAudio::update()
	{
        if(!isValid() || !isPlaying())
		{
            return false;
		}
        int processed = 0;
        bool active = true;

		Mutex.lock();
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
		Mutex.unlock();
		return active;
    }

	void cAudio::release()
    {
		Mutex.lock();
		//Stops the audio Source
		alSourceStop(Source);
		empty();
		//Deletes the source
		alDeleteSources(1, &Source);
		//deletes the last filled buffer
		alDeleteBuffers(NUM_BUFFERS, Buffers);
		checkError();
		Mutex.unlock();
		getLogger()->logDebug("Audio Source", "Audio source released.");
    }

	const bool cAudio::isValid() const
	{
		bool state = (Decoder != 0);
        return state;
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
		Mutex.lock();
        alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setVelocity(const cVector3& velocity)
	{
		Mutex.lock();
        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setDirection(const cVector3& direction)
	{
		Mutex.lock();
        alSource3f(Source, AL_DIRECTION, direction.x, direction.y, direction.z);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setRolloffFactor(const float& rolloff)
	{
		Mutex.lock();
        alSourcef(Source, AL_ROLLOFF_FACTOR, rolloff);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setStrength(const float& soundstrength)
	{
		float inverseStrength = 0.0f;
		if(soundstrength > 0.0f)
			inverseStrength = 1.0f / soundstrength;

		Mutex.lock();
        alSourcef(Source, AL_ROLLOFF_FACTOR, inverseStrength);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setMinDistance(const float& minDistance)
	{
		Mutex.lock();
        alSourcef(Source, AL_REFERENCE_DISTANCE, minDistance);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setMaxDistance(const float& maxDistance)
	{
		Mutex.lock();
        alSourcef(Source, AL_MAX_DISTANCE, maxDistance);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setPitch(const float& pitch)
	{
		Mutex.lock();
        alSourcef (Source, AL_PITCH, pitch);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setVolume(const float& volume)
	{
		Mutex.lock();
        alSourcef(Source, AL_GAIN, volume);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setMinVolume(const float& minVolume)
	{
		Mutex.lock();
        alSourcef(Source, AL_MIN_GAIN, minVolume);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setMaxVolume(const float& maxVolume)
	{
		Mutex.lock();
        alSourcef(Source, AL_MAX_GAIN, maxVolume);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setInnerConeAngle(const float& innerAngle)
	{
		Mutex.lock();
        alSourcef(Source, AL_CONE_INNER_ANGLE, innerAngle);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setOuterConeAngle(const float& outerAngle)
	{
		Mutex.lock();
        alSourcef(Source, AL_CONE_OUTER_ANGLE, outerAngle);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setOuterConeVolume(const float& outerVolume)
	{
		Mutex.lock();
        alSourcef(Source, AL_CONE_OUTER_GAIN, outerVolume);
		checkError();
		Mutex.unlock();
	}

	void cAudio::setDopplerStrength(const float& dstrength)
	{
		Mutex.lock();
        alSourcef(Source, AL_DOPPLER_FACTOR, dstrength);
		checkError();
		Mutex.unlock();
    }

	void cAudio::setDopplerVelocity(const cVector3& dvelocity)
	{
		Mutex.lock();
        alSource3f(Source, AL_DOPPLER_VELOCITY, dvelocity.x, dvelocity.y, dvelocity.z);
		checkError();
		Mutex.unlock();
    }

	void cAudio::move(const cVector3& position)
	{
		Mutex.lock();
		cVector3 oldPos = getPosition();
		cVector3 velocity = position - oldPos;

        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkError();
		Mutex.unlock();
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

	void cAudio::checkError()
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
        }
    }

    bool cAudio::stream(ALuint buffer)
    {
        if(Decoder)
        {
	        //stores the caculated data into buffer that is passed to output.
			size_t totalread = 0;
			unsigned int errorcount = 0;
	        char tempbuffer[BUFFER_SIZE];
			while( totalread < BUFFER_SIZE )
			{
				char tempbuffer2[BUFFER_SIZE];
				int actualread = Decoder->readAudioData(tempbuffer2, BUFFER_SIZE-totalread);
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
            alBufferData(buffer, Decoder->getFormat(), tempbuffer, totalread, Decoder->getFrequency());
			checkError();
            return true;
        }
		return false;
    }
}
