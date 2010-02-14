// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IAUDIOEFFECTS_H
#define IAUDIOEFFECTS_H

#include "IEffect.h"
#include "IFilter.h"
#include "cAudioDefines.h"

#ifdef CAUDIO_EFX_ENABLED

namespace cAudio
{
    class IAudioEffects
    {
    public:
		IAudioEffects() {}
        virtual ~IAudioEffects() {}
		
		//*! Creates a IEffect pointer.
		virtual IEffect* createEffect() = 0;
		
		//*! Creates a IEffect pointer.
		virtual IFilter* createFilter() = 0;

		//*! Returns the max number of effects supported
		virtual unsigned int getMaxEffectsSupported() const = 0;

		/*! Checks to see if the given effect type is supported
		\param type: the effect type to be checked
		\return True if the effect is supported, False if the effect isn't supported
		*/
		virtual bool isEffectSupported(const EffectTypes& type) const = 0;
		
		/*! Checks to see if the given filter type is supported
		\param type: the filter type to be checked
		\return True if the filter is supported, False if the filter isn't supported
		*/
		virtual bool isFilterSupported(const FilterTypes& type) const = 0;

		virtual bool addEAXReverbEffectPreset(const char* name, const sEAXReverbParameters& setting) = 0;
		virtual bool addReverbEffectPreset(const char* name, const sReverbParameters& setting) = 0;
		virtual bool addChorusEffectPreset(const char* name, const sChorusParameters& setting) = 0;
		virtual bool addDistortionEffectPreset(const char* name, const sDistortionParameters& setting) = 0;
		virtual bool addEchoEffectPreset(const char* name, const sEchoParameters& setting) = 0;
		virtual bool addFlangerEffectPreset(const char* name, const sFlangerParameters& setting) = 0;
		virtual bool addFrequencyShiftEffectPreset(const char* name, const sFrequencyShiftParameters& setting) = 0;
		virtual bool addVocalMorpherEffectPreset(const char* name, const sVocalMorpherParameters& setting) = 0;
		virtual bool addPitchShifterEffectPreset(const char* name, const sPitchShifterParameters& setting) = 0;
		virtual bool addRingModulatorEffectPreset(const char* name, const sRingModulatorParameters& setting) = 0;
		virtual bool addAutowahEffectPreset(const char* name, const sAutowahParameters& setting) = 0;
		virtual bool addCompressorEffectPreset(const char* name, const sCompressorParameters& setting) = 0;
		virtual bool addEqualizerEffectPreset(const char* name, const sEqualizerParameters& setting) = 0;

		virtual sEAXReverbParameters getEAXReverbEffectPreset(const char* name) = 0;
		virtual sReverbParameters getReverbEffectPreset(const char* name) = 0;
		virtual sChorusParameters getChorusEffectPreset(const char* name) = 0;
		virtual sDistortionParameters getDistortionEffectPreset(const char* name) = 0;
		virtual sEchoParameters getEchoEffectPreset(const char* name) = 0;
		virtual sFlangerParameters getFlangerEffectPreset(const char* name) = 0;
		virtual sFrequencyShiftParameters getFrequencyShiftEffectPreset(const char* name) = 0;
		virtual sVocalMorpherParameters getVocalMorpherEffectPreset(const char* name) = 0;
		virtual sPitchShifterParameters getPitchShifterEffectPreset(const char* name) = 0;
		virtual sRingModulatorParameters getRingModulatorEffectPreset(const char* name) = 0;
		virtual sAutowahParameters getAutowahEffectPreset(const char* name) = 0;
		virtual sCompressorParameters getCompressorEffectPreset(const char* name) = 0;
		virtual sEqualizerParameters getEqualizerEffectPreset(const char* name) = 0;

		virtual void removePreset(const EffectTypes& type, const char* name) = 0;
		virtual bool isPresetRegistered(const EffectTypes& type, const char* name) = 0;
		virtual void removeAllPresets(const EffectTypes& type) = 0;

    protected:
    private:
    };
};

#endif

#endif //! IAUDIOEFFECTS_H