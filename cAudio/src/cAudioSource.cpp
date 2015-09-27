// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cAudioSource.h"
#include "cAudio.h"
#include "cLogger.h"
#include "cFilter.h"
#include "cEffect.h"
#include "cAudioSleep.h"
#include <string.h>
#include <algorithm>

#if CAUDIO_EFX_ENABLED == 1
#include "cOpenALDeviceContext.h"
#endif

namespace
{
    // return buffer length in seconds
    ALfloat GetBufferLength(ALuint buffer)
    {
        ALint size, bits, channels, freq;

        alGetBufferi(buffer, AL_SIZE, &size);
        alGetBufferi(buffer, AL_BITS, &bits);
        alGetBufferi(buffer, AL_CHANNELS, &channels);
        alGetBufferi(buffer, AL_FREQUENCY, &freq);
        if(alGetError() != AL_NO_ERROR || !bits || !channels || !freq)
            return -1.0f;

        return (ALfloat)((ALuint)size/channels/(bits/8)) / (ALfloat)freq;
    }

    ALint GetBufferSize(ALuint buffer)
    {
        ALint size;
        alGetBufferi(buffer, AL_SIZE, &size);
        return size;
    }
}

namespace cAudio
{

#if CAUDIO_EFX_ENABLED == 1
    cAudioSource::cAudioSource(IAudioDecoder* decoder, IAudioDeviceContext* context, cEFXFunctions* oALFunctions)
		: cAudioSourceBase(context), Decoder(decoder), Loop(false), Valid(false),
		EFX(oALFunctions), Filter(NULL), EffectSlotsAvailable(0), LastFilterTimeStamp(0)
#else
	cAudioSource::cAudioSource(IAudioDecoder* decoder, IAudioDeviceContext* context)
        : cAudioSourceBase(context), Decoder(decoder), Loop(false), Valid(false)
#endif
    {
		cAudioMutexBasicLock lock(Mutex);

        BufferPosition = 0;
        BufferTime = 0;
		for(int i=0; i<CAUDIO_SOURCE_NUM_BUFFERS; ++i) {
			Buffers[i] = 0;
        }

#if CAUDIO_EFX_ENABLED == 1
		for(int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			Effects[i] = NULL;

		for(int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			LastEffectTimeStamp[i] = 0;
#endif

		if(Decoder)
			Decoder->grab();

        ALboolean state = alIsSource(Source);

        if (state)
        {
            //Generates 3 buffers for the ogg file
            alGenBuffers(CAUDIO_SOURCE_NUM_BUFFERS, Buffers);
            state = !checkALError();
        }

#if CAUDIO_EFX_ENABLED == 1
		Valid = state && (Decoder != NULL) && (Context != NULL) && (EFX != NULL);

		int numSlots = 0;
		ALCdevice* device = alcGetContextsDevice(((cOpenALDeviceContext*)Context)->getOpenALContext());
		alcGetIntegerv(device, ALC_MAX_AUXILIARY_SENDS, 1, &numSlots);

		EffectSlotsAvailable = (numSlots <= CAUDIO_SOURCE_MAX_EFFECT_SLOTS) ? numSlots : CAUDIO_SOURCE_MAX_EFFECT_SLOTS;
#else
		Valid = state && (Decoder != NULL) && (Context != NULL);
#endif
    }

    cAudioSource::~cAudioSource()
    {
		cAudioMutexBasicLock lock(Mutex);

#if CAUDIO_EFX_ENABLED == 1
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
		
		//Stops the audio Source
		alSourceStop(Source);
        checkALError();
		empty();
        alSourcei(Source, AL_BUFFER, 0);
		//deletes the last filled buffer
		alDeleteBuffers(CAUDIO_SOURCE_NUM_BUFFERS, Buffers);
		checkALError();

		if(Decoder) {
			Decoder->drop();
            Decoder = NULL;
        }
    }

    cAudioSourceBase::cAudioSourceBase(IAudioDeviceContext* context) : 
        Context(context), Volume(1.f), Source(0) 
    {
        alGenSources(1, &Source);
        checkALError();
        setVolume(Volume);
    }

    cAudioSourceBase::~cAudioSourceBase()
    {
        alSourceStop(Source);
        checkALError();
		alDeleteSources(1, &Source);
        checkALError();

		getLogger()->logDebug("Audio Source", "Audio source released.");
		signalEvent(ON_RELEASE);
    }


	bool cAudioSource::drop()
	{
		--RefCount;
		if (RefCount == 0)
		{
			Context->getAudioManager()->release(this);
			return true;
		}
		return false;
	}

	bool cAudioSource::play()
	{
		cAudioMutexBasicLock lock(Mutex);
		if (!isPaused())
        {
            int queueSize = 0;
			//Purges all buffers from the source
			alSourcei(Source, AL_BUFFER, 0);
            BufferPosition = Decoder->getCurrentPosition();
            BufferTime = Decoder->getCurrentTime();
			checkALError();
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
			checkALError();
        }
#if CAUDIO_EFX_ENABLED == 1
		updateFilter();
		for(unsigned int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
			updateEffect(i);
#endif
        alSourcePlay(Source);
		checkALError();
		getLogger()->logDebug("Audio Source", "Source playing.");
		signalEvent(ON_PLAY);
		oldState = AL_PLAYING;
        return true;
    }

	bool cAudioSource::play2d(const bool& toLoop)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcei(Source, AL_SOURCE_RELATIVE, true);
        loop(toLoop);
        bool state = play();
		checkALError();
		return state;
    }

	bool cAudioSource::play3d(const cVector3& position, const float& soundstr, const bool& toLoop)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcei(Source, AL_SOURCE_RELATIVE, false);
        setPosition(position);
        setStrength(soundstr);
        loop(toLoop);
        bool state = play();
		checkALError();
		return state;
    }

	void cAudioSource::pause()
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcePause(Source);
		checkALError();
		getLogger()->logDebug("Audio Source", "Source paused.");
		signalEvent(ON_PAUSE);
		oldState = AL_PAUSED;
    }

	void cAudioSource::stop()
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourceStop(Source);

		//INFO:FIXED EXTREME SLOWDOWN ON IPHONE
		int queued = 0;
		alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);
		while ( queued-- )
		{
			ALuint buffer;
			alSourceUnqueueBuffers(Source, 1, &buffer);
		}

		//Resets the audio to the beginning
		Decoder->setPosition(0, false);
		checkALError();
		getLogger()->logDebug("Audio Source", "Source stopped.");
		signalEvent(ON_STOP);
		oldState = AL_STOPPED;
    }

	void cAudioSource::loop(const bool& loop)
	{
		cAudioMutexBasicLock lock(Mutex);
        Loop = loop;
    }

	bool cAudioSource::seek(const float& seconds, bool relative)
	{
		bool state = false;
		cAudioMutexBasicLock lock(Mutex);
        if(Decoder->isSeekingSupported())
        {
			state = Decoder->seek(seconds, relative);

            BufferPosition = Decoder->getCurrentPosition();
            BufferTime = Decoder->getCurrentTime();

            int queued = 0;
            alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);
            if (queued) {
                BufferPosition -= queued * GetBufferSize(Buffers[0]);
                BufferTime -= queued * GetBufferLength(Buffers[0]);
            }
        }
		return state;
    }

	float cAudioSource::getTotalAudioTime()
	{
		return Decoder->getTotalTime();
	}

	int cAudioSource::getTotalAudioSize()
	{
		return Decoder->getTotalSize();
	}

	int cAudioSource::getCompressedAudioSize()
	{
		return Decoder->getCompressedSize();
	}

	float cAudioSource::getCurrentAudioTime()
	{
        float time = -1;
        alGetSourcef(Source, AL_SEC_OFFSET, &time);
        return BufferTime + time;
	}

	int cAudioSource::getCurrentAudioPosition()
	{
        int offset = -1;
        alGetSourcei(Source, AL_BYTE_OFFSET, &offset);
		return BufferPosition + offset;
	}

	int cAudioSource::getCurrentCompressedAudioPosition()
	{
		return Decoder->getCurrentCompressedPosition();
	}

	bool cAudioSource::update()
	{
		cAudioMutexBasicLock lock(Mutex);

		int processed = 0;
		bool active = true;
        if(isValid() || isPlaying())
		{
#if CAUDIO_EFX_ENABLED == 1
			updateFilter();
			for(unsigned int i=0; i<CAUDIO_SOURCE_MAX_EFFECT_SLOTS; ++i)
				updateEffect(i);
#endif

			//gets the sound source processed buffers
			alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
            checkALError();
            
			//while there is more data refill buffers with audio data.
			while (processed--)
			{
				ALuint buffer;
				alSourceUnqueueBuffers(Source, 1, &buffer);
                BufferPosition += GetBufferSize(buffer);
                BufferTime += GetBufferLength(buffer);

				if (checkALError()) 
				{
					processed++;
					cAudioSleep(1);
					continue;
				}

				active = stream(buffer);

				//if more in stream continue playing.
				if(active)
				{
					alSourceQueueBuffers(Source, 1, &buffer);
				}

				if (checkALError()) 
				{
					processed++;
					cAudioSleep(1);
					continue;
				}
			}

			signalEvent(ON_UPDATE);
		}

		ALenum state;
		alGetSourcei(Source, AL_SOURCE_STATE, &state);
        checkALError();
		if(state == AL_STOPPED && oldState != state)
		{
			//Resets the audio to the beginning
			Decoder->setPosition(0, false);
			getLogger()->logDebug("Audio Source", "Source stopped.");
			signalEvent(ON_STOP);
			oldState = state;
		}

		return active;
    }

	bool cAudioSource::isValid() const
	{
        return Valid;
	}

	bool cAudioSourceBase::isPlaying() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        checkALError();
        return (state == AL_PLAYING);
    }

	bool cAudioSourceBase::isPaused() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        checkALError();
        return (state == AL_PAUSED);
    }

	bool cAudioSourceBase::isStopped() const
	{
		ALenum state = 0;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        checkALError();
		return (state == AL_STOPPED);
    }

	bool cAudioSource::isLooping() const
	{
		return Loop;
	}

	void cAudioSourceBase::setPosition(const cVector3& position)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkALError();
    }

	void cAudioSourceBase::setVelocity(const cVector3& velocity)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		checkALError();
    }

	void cAudioSourceBase::setDirection(const cVector3& direction)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_DIRECTION, direction.x, direction.y, direction.z);
		checkALError();
    }

	void cAudioSourceBase::setRolloffFactor(const float& rolloff)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_ROLLOFF_FACTOR, rolloff);
		checkALError();
    }

	void cAudioSourceBase::setStrength(const float& soundstrength)
	{
		float inverseStrength = 0.0f;
		if(soundstrength > 0.0f)
			inverseStrength = 1.0f / soundstrength;

		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_ROLLOFF_FACTOR, inverseStrength);
		checkALError();
    }

	void cAudioSourceBase::setMinDistance(const float& minDistance)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_REFERENCE_DISTANCE, minDistance);
		checkALError();
	}

	void cAudioSourceBase::setMaxAttenuationDistance(const float& maxDistance)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MAX_DISTANCE, maxDistance);
		checkALError();
	}

	void cAudioSourceBase::setPitch(const float& pitch)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef (Source, AL_PITCH, pitch);
		checkALError();
    }

	void cAudioSourceBase::setVolume(const float& volume)
	{
		cAudioMutexBasicLock lock(Mutex);
		Volume = volume;
        alSourcef(Source, AL_GAIN, Volume * Context->getAudioManager()->getMasterVolume());
		checkALError();
    }

	void cAudioSourceBase::setMinVolume(const float& minVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MIN_GAIN, minVolume);
		checkALError();
	}

	void cAudioSourceBase::setMaxVolume(const float& maxVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_MAX_GAIN, maxVolume);
		checkALError();
	}

	void cAudioSourceBase::setInnerConeAngle(const float& innerAngle)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_INNER_ANGLE, innerAngle);
		checkALError();
	}

	void cAudioSourceBase::setOuterConeAngle(const float& outerAngle)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_OUTER_ANGLE, outerAngle);
		checkALError();
	}

	void cAudioSourceBase::setOuterConeVolume(const float& outerVolume)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_CONE_OUTER_GAIN, outerVolume);
		checkALError();
	}

	void cAudioSourceBase::setDopplerStrength(const float& dstrength)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSourcef(Source, AL_DOPPLER_FACTOR, dstrength);
		checkALError();
    }

	void cAudioSourceBase::setDopplerVelocity(const cVector3& dvelocity)
	{
		cAudioMutexBasicLock lock(Mutex);
        alSource3f(Source, AL_DOPPLER_VELOCITY, dvelocity.x, dvelocity.y, dvelocity.z);
		checkALError();
    }

	void cAudioSourceBase::move(const cVector3& position)
	{
		cAudioMutexBasicLock lock(Mutex);
		cVector3 oldPos = getPosition();
		cVector3 velocity = position - oldPos;

        alSource3f(Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSource3f(Source, AL_POSITION, position.x, position.y, position.z);
		checkALError();
	}

	cVector3 cAudioSourceBase::getPosition() const
	{
		cVector3 position;
		alGetSourcefv(Source, AL_POSITION, &position.x);
        checkALError();
		return position;
	}

	cVector3 cAudioSourceBase::getVelocity() const
	{
		cVector3 velocity;
		alGetSourcefv(Source, AL_VELOCITY, &velocity.x);
		return velocity;
	}

	cVector3 cAudioSourceBase::getDirection() const
	{
		cVector3 direction;
		alGetSourcefv(Source, AL_DIRECTION, &direction.x);
        checkALError();
		return direction;
	}

	float cAudioSourceBase::getRolloffFactor() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_ROLLOFF_FACTOR, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getStrength() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_ROLLOFF_FACTOR, &value);
        checkALError();
        
		float inverseStrength = 0.0f;
		if(value > 0.0f)
			inverseStrength = 1.0f / value;

		return inverseStrength;
	}

	float cAudioSourceBase::getMinDistance() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_REFERENCE_DISTANCE, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getMaxDistance() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MAX_DISTANCE, &value);
        checkALError();
		return value;
	}

    bool cAudioSourceBase::isRelative() const
    {
        int relative = 0;
        alGetSourcei(Source, AL_SOURCE_RELATIVE, &relative);
        return relative;
    }

    float cAudioSourceBase::calculateGain() const
    {
        // OpenAL Inverse Distance Clamped Model
        // distance = max(distance,AL_REFERENCE_DISTANCE);
        // distance = min(distance,AL_MAX_DISTANCE);
        // gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE +
        //                                 AL_ROLLOFF_FACTOR *
        //                                 (distance – AL_REFERENCE_DISTANCE));

        cVector3 lpos = Context->getAudioManager()->getListener()->getPosition();
        cVector3 pos  = getPosition();

        float refDist =  getMinDistance();
        float dist = 0.f;
        dist = isRelative() ? pos.length() : (pos - lpos).length();
        dist = std::max(dist, refDist);
        dist = std::min(dist, getMaxDistance());
        float gain = refDist / (refDist + getRolloffFactor() * (dist - refDist));
        return gain * getVolume();
    }

	float cAudioSourceBase::getPitch() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_PITCH, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getVolume() const
	{
		return Volume;
	}

	float cAudioSourceBase::getMinVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MIN_GAIN, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getMaxVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_MAX_GAIN, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getInnerConeAngle() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_INNER_ANGLE, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getOuterConeAngle() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_OUTER_ANGLE, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getOuterConeVolume() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_CONE_OUTER_GAIN, &value);
        checkALError();
		return value;
	}

	float cAudioSourceBase::getDopplerStrength() const
	{
		float value = 0.0f;
		alGetSourcef(Source, AL_DOPPLER_FACTOR, &value);
        checkALError();
		return value;
	}

	cVector3 cAudioSourceBase::getDopplerVelocity() const
	{
		cVector3 velocity;
		alGetSourcefv(Source, AL_DOPPLER_VELOCITY, &velocity.x);
        checkALError();
		return velocity;
	}

#if CAUDIO_EFX_ENABLED == 1
	unsigned int cAudioSource::getNumEffectSlotsAvailable() const
	{
		return EffectSlotsAvailable;
	}

	bool cAudioSource::attachEffect(unsigned int slot, IEffect* effect)
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

	void cAudioSource::removeEffect(unsigned int slot)
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

	bool cAudioSource::attachFilter(IFilter* filter)
	{
		cAudioMutexBasicLock lock(Mutex);
		Filter = filter;

		if(Filter)
			Filter->grab();

		updateFilter();
		return true;
	}

	void cAudioSource::removeFilter()
	{
		cAudioMutexBasicLock lock(Mutex);
		if(Filter)
			Filter->drop();
		Filter = NULL;
		LastFilterTimeStamp = 0;
		updateFilter(true);
	}
#endif

    void cAudioSource::empty()
    {
        int queued = 0;
        alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);
        checkALError();
        
        while (queued--)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(Source, 1, &buffer);
			checkALError();
        }
    }

    bool cAudioSource::stream(ALuint buffer)
    {
        if(!Decoder)
            return false;
        
        //stores the calculated data into buffer that is passed to output.
        size_t totalread = 0;
        unsigned int errorcount = 0;
        char tempbuffer[CAUDIO_SOURCE_BUFFER_SIZE];
        while( totalread < CAUDIO_SOURCE_BUFFER_SIZE )
        {
            char tempbuffer2[CAUDIO_SOURCE_BUFFER_SIZE];
            Mutex.unlock();    // this can take a long time
            int actualread = Decoder->readAudioData(tempbuffer2, CAUDIO_SOURCE_BUFFER_SIZE-totalread);
            Mutex.lock();
            
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
                {
                    stop();
                    break;
                }
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
        checkALError();
        return true;
    }

#if CAUDIO_EFX_ENABLED == 1
	void cAudioSource::updateFilter(bool remove)
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
						checkALError();
						return;
					}
				}
				return;
			}
		}
		alSourcei(Source, AL_DIRECT_FILTER, AL_FILTER_NULL);
		checkALError();
	}

	void cAudioSource::updateEffect(unsigned int slot, bool remove)
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
							checkALError();
							return;
						}
					}
					return;
				}
			}
			alSource3i(Source, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, slot, AL_FILTER_NULL);
			checkALError();
		}
	}
#endif

	void cAudioSourceBase::registerEventHandler(ISourceEventHandler* handler)
	{
		if(handler)
		{
		    cAudioMutexBasicLock lock(Mutex);
			eventHandlerList.push_back(handler);
		}
	}

	void cAudioSourceBase::unRegisterEventHandler(ISourceEventHandler* handler)
	{
		if(handler)
		{
		    cAudioMutexBasicLock lock(Mutex);
            for(int i=0; i<eventHandlerList.size(); i++) {
                if(eventHandlerList[i] == handler)
                    eventHandlerList.erase(eventHandlerList.begin() + i);
            }
		}
	}

	void cAudioSourceBase::unRegisterAllEventHandlers()
	{
	    cAudioMutexBasicLock lock(Mutex);
		eventHandlerList.clear();
	}

	void cAudioSourceBase::signalEvent(Events sevent)
	{
		cAudioMutexBasicLock lock(Mutex);
		if(eventHandlerList.empty())
            return;

        size_t size = eventHandlerList.size();

        for(int i=0; i<size; )
        {
            ISourceEventHandler *handler = eventHandlerList[i];

            switch(sevent)
            {
            case ON_UPDATE:  handler->onUpdate(); break;
            case ON_RELEASE: handler->onRelease(); break;
            case ON_PLAY:    handler->onPlay(); break;
            case ON_PAUSE:   handler->onPause(); break;
            case ON_STOP:    handler->onStop(); break;
            }
     
            // handler may have unregistered itself
            if(size == eventHandlerList.size()) {
                i++;
            } else {
                size = eventHandlerList.size();
            }
        }
    }
}
