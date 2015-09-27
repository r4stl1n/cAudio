// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#pragma once

#include <list>
#include <string>
#include <vector>

#include "cOpenALUtil.h"
#include "cMutex.h"
#include "cEFXFunctions.h"
#include "cMemoryOverride.h"
#include "IAudioSource.h"
#include "cVector3.h"
#include "ILogger.h"
#include "cSTLAllocator.h"
#include "IAudioDeviceContext.h"

namespace cAudio
{
    
    class cAudioSourceBase : public IAudioSource, public cMemoryOverride
    {
    public:

		enum Events{
			ON_UPDATE,
			ON_RELEASE,
			ON_PLAY,
			ON_PAUSE,
			ON_STOP,
		};

        cAudioSourceBase(IAudioDeviceContext* context);
        virtual ~cAudioSourceBase();

		virtual bool isPlaying() const;
		virtual bool isPaused() const;
		virtual bool isStopped() const;

		virtual void setPosition(const cVector3& position);
		virtual void setVelocity(const cVector3& velocity);
		virtual void setDirection(const cVector3& direction);

		virtual void setRolloffFactor(const float& rolloff);
		virtual void setStrength(const float& soundstrength);
		virtual void setMinDistance(const float& minDistance);
		virtual void setMaxAttenuationDistance(const float& maxDistance);

		virtual void setPitch(const float& pitch);
		virtual void setVolume(const float& volume);
		virtual void setMinVolume(const float& minVolume);
		virtual void setMaxVolume(const float& maxVolume);

		virtual void setInnerConeAngle(const float& innerAngle);
		virtual void setOuterConeAngle(const float& outerAngle);
		virtual void setOuterConeVolume(const float& outerVolume);

		virtual void setDopplerStrength(const float& dstrength); 
		virtual void setDopplerVelocity(const cVector3& dvelocity);

		virtual void move(const cVector3& position);

		virtual cVector3 getPosition() const;
		virtual cVector3 getVelocity() const;
		virtual cVector3 getDirection() const;

		virtual float getRolloffFactor() const;
		virtual float getStrength() const;
		virtual float getMinDistance() const;
		virtual float getMaxDistance() const;

        virtual bool isRelative() const;
        virtual float calculateGain() const;

		virtual float getPitch() const;
		virtual float getVolume() const;
		virtual float getMinVolume() const;
		virtual float getMaxVolume() const;

		virtual float getInnerConeAngle() const;
		virtual float getOuterConeAngle() const;
		virtual float getOuterConeVolume() const;

		virtual float getDopplerStrength() const;
		virtual cVector3 getDopplerVelocity() const;

		virtual void registerEventHandler(ISourceEventHandler* handler);
		virtual void unRegisterEventHandler(ISourceEventHandler* handler);
		virtual void unRegisterAllEventHandlers();

    protected:
		//! Mutex for thread synchronization
		cAudioMutex Mutex;

		//! Signals a event to all event handlers
		void signalEvent(Events sevent);

		//! The context that owns this source
		IAudioDeviceContext* Context;
		
		//! Holds the current volume
		float Volume;

 		//! OpenAL source
		ALuint Source; 

		ALenum oldState;

		//! List of registered event handlers
		cAudioVector<ISourceEventHandler*>::Type eventHandlerList;
    };

    class cAudioSource : public cAudioSourceBase {
    public:
#if CAUDIO_EFX_ENABLED == 1
		cAudioSource(IAudioDecoder* decoder, IAudioDeviceContext* context, cEFXFunctions* oALFunctions);
#else
		cAudioSource(IAudioDecoder* decoder, IAudioDeviceContext* context);
#endif
		~cAudioSource();

		virtual bool play();
		virtual bool play2d(const bool& toLoop = false);
		virtual bool play3d(const cVector3& position, const float& soundstr = 1.0 , const bool& toLoop = false);
		
		virtual void pause();    
		virtual void stop();
		virtual void loop(const bool& toLoop);
		virtual bool seek(const float& seconds, bool relative = false);

        virtual bool setBuffer(IAudioBuffer* buffer) { return false; }
        virtual IAudioBuffer *getBuffer() { return NULL; }

		virtual float getTotalAudioTime();
		virtual int getTotalAudioSize();
		virtual int getCompressedAudioSize();

		virtual float getCurrentAudioTime();
		virtual int getCurrentAudioPosition();
		virtual int getCurrentCompressedAudioPosition();

		virtual bool update();

		virtual bool isValid() const;
		virtual bool isLooping() const;

		virtual bool drop(); //! Override the default behavior

#if CAUDIO_EFX_ENABLED == 1
		virtual unsigned int getNumEffectSlotsAvailable() const;
		virtual bool attachEffect(unsigned int slot, IEffect* effect);
		virtual void removeEffect(unsigned int slot);

		virtual bool attachFilter(IFilter* filter);
		virtual void removeFilter();
#endif

	private:
		//! Empties the current working buffer queue
		void empty();
		//! Streams audio data from the decoder into a buffer
		bool stream(ALuint buffer);

		//! Internal audio buffers
		ALuint Buffers[CAUDIO_SOURCE_NUM_BUFFERS]; 

		//! cAudio decoder being used to stream data
		IAudioDecoder* Decoder;

		//! Stores whether the source is to loop the audio stream
		bool Loop;
		//! Stores whether the source is ready to be used
		bool Valid;

        //! position of first buffer in seconds
        float BufferTime;
        //! position of first buffer in bytes
        int BufferPosition;

#if CAUDIO_EFX_ENABLED == 1
		//! Holds pointers to all the EFX related functions
		cEFXFunctions* EFX;
		//! Updates the attached filter
		void updateFilter(bool remove = false);
		//! Updates the effect attached to a specific slot
		void updateEffect(unsigned int slot, bool remove = false);

		//! Stores the effects attached to this source
		IEffect* Effects[CAUDIO_SOURCE_MAX_EFFECT_SLOTS];
		//! Stores the last updated time stamps for the attached effects
		unsigned int LastEffectTimeStamp[CAUDIO_SOURCE_MAX_EFFECT_SLOTS];

		//! Stores the attached direct feed filter
		IFilter* Filter;
		//! Stores the last updated time stamp for the attached filter
		unsigned int LastFilterTimeStamp;

		//! Number of effects supported by the OpenAL Context
		unsigned int EffectSlotsAvailable;
#endif
    };
};
