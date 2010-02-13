// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "../Headers/cAudioEffects.h"
#include "../Headers/cFilter.h"
#include "../Headers/cEffect.h"

#ifdef CAUDIO_EFX_ENABLED

//Number of effect slots to try to create
#define EFFECT_SLOTS_TO_TEST 256

namespace cAudio
{

cAudioEffects::cAudioEffects() : MaxEffectsSupported(0)
{
	for(int i=0; i<EET_COUNT; ++i)
		SupportedEffects[i] = false;

	for(int i=0; i<EFT_COUNT; ++i)
		SupportedFilters[i] = false;
}

cAudioEffects::~cAudioEffects()
{

}

IEffect* cAudioEffects::createEffect()
{
	cAudioMutexBasicLock lock(Mutex);
	IEffect* effect = new cEffect(&EFXInterface);

	if(effect && effect->isValid())
		return effect;

	return NULL;
}

IFilter* cAudioEffects::createFilter()
{
	cAudioMutexBasicLock lock(Mutex);
	IFilter* filter = new cFilter(&EFXInterface);

	if(filter && filter->isValid())
		return filter;

	return NULL;
}

unsigned int cAudioEffects::getMaxEffectsSupported() const
{
	return MaxEffectsSupported;
}

bool cAudioEffects::isEffectSupported(const EffectTypes& type) const
{
	return SupportedEffects[type];
}

bool cAudioEffects::isFilterSupported(const FilterTypes& type) const
{
	return SupportedFilters[type];
}

cEFXFunctions* cAudioEffects::getEFXInterface()
{
	return &EFXInterface;
}

void cAudioEffects::checkEFXSupportDetails()
{
	cAudioMutexBasicLock lock(Mutex);
	if(EFXInterface.Supported)
	{
		cAudioMutexBasicLock lock(EFXInterface.Mutex);

		//Count the number of effect slots this device supports (limits the max number of effects available)
		unsigned int count = 0;
		ALuint effectSlots[EFFECT_SLOTS_TO_TEST];
		for(count=0; count<EFFECT_SLOTS_TO_TEST; ++count)
		{
			EFXInterface.alGenAuxiliaryEffectSlots(1, &effectSlots[count]);
			if(alGetError() != AL_NO_ERROR)
				break;
		}

		MaxEffectsSupported = count;

		//Check what effects are supported
		ALuint Effect;
		EFXInterface.alGenEffects(1, &Effect);
		if (alGetError() == AL_NO_ERROR)
		{
			SupportedEffects[EET_NULL] = true;

			// Try setting Effect Type to known Effects
			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
			SupportedEffects[EET_REVERB] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
			SupportedEffects[EET_EAX_REVERB] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_CHORUS);
			SupportedEffects[EET_CHORUS] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_DISTORTION);
			SupportedEffects[EET_DISTORTION] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_ECHO);
			SupportedEffects[EET_ECHO] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_FLANGER);
			SupportedEffects[EET_FLANGER] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_FREQUENCY_SHIFTER);
			SupportedEffects[EET_FREQUENCY_SHIFTER] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_VOCAL_MORPHER);
			SupportedEffects[EET_VOCAL_MORPHER] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_PITCH_SHIFTER);
			SupportedEffects[EET_PITCH_SHIFTER] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_RING_MODULATOR);
			SupportedEffects[EET_RING_MODULATOR] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_AUTOWAH);
			SupportedEffects[EET_AUTOWAH] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_COMPRESSOR);
			SupportedEffects[EET_COMPRESSOR] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alEffecti(Effect, AL_EFFECT_TYPE, AL_EFFECT_EQUALIZER);
			SupportedEffects[EET_EQUALIZER] = (alGetError() == AL_NO_ERROR);
		}

		// Delete Effect
		EFXInterface.alDeleteEffects(1, &Effect);

		// Generate a Filter to use to determine what Filter Types are supported
		ALuint Filter;
		EFXInterface.alGenFilters(1, &Filter);
		if (alGetError() == AL_NO_ERROR)
		{
			SupportedFilters[EFT_NULL] = true;

			// Try setting the Filter type to known Filters
			EFXInterface.alFilteri(Filter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
			SupportedFilters[EFT_LOWPASS] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alFilteri(Filter, AL_FILTER_TYPE, AL_FILTER_HIGHPASS);
			SupportedFilters[EFT_HIGHPASS] = (alGetError() == AL_NO_ERROR);

			EFXInterface.alFilteri(Filter, AL_FILTER_TYPE, AL_FILTER_BANDPASS);
			SupportedFilters[EFT_BANDPASS] = (alGetError() == AL_NO_ERROR);
		}

		// Delete Filter
		EFXInterface.alDeleteFilters(1, &Filter);

		//Cleanup the slots we created
		while(count > 0)
		{
			EFXInterface.alDeleteAuxiliaryEffectSlots(1, &effectSlots[--count]);
		}
	}
}

};

#endif