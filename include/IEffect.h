// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IEFFECT_H
#define IEFFECT_H

#include "IRefCounted.h"
#include "IFilter.h"
#include "IEffectParameters.h"

namespace cAudio
{
	enum EffectTypes
	{
		EET_NULL,
		EET_EAX_REVERB,
		EET_REVERB,
		EET_CHORUS,
		EET_DISTORTION,
		EET_ECHO,
		EET_FLANGER,
		EET_FREQUENCY_SHIFTER,
		EET_VOCAL_MORPHER,
		EET_PITCH_SHIFTER,
		EET_RING_MODULATOR,
		EET_AUTOWAH,
		EET_COMPRESSOR,
		EET_EQUALIZER,
		EET_COUNT
	};

	class IEffect : public IRefCounted
	{
	public:
		IEffect() {  }
		virtual ~IEffect() {  }

		virtual const EffectTypes& getType() const = 0;
		virtual void setType(const EffectTypes& type) = 0;

		virtual const sEAXReverbParameters& getEAXReverbParameters() const = 0;
		virtual void setEAXReverbParameters(const sEAXReverbParameters& param) = 0;

		virtual const sReverbParameters& getReverbParameters() const = 0;
		virtual void setReverbParameters(const sReverbParameters& param) = 0;

		virtual const sChorusParameters& getChorusParameters() const = 0;
		virtual void setChorusParameters(const sChorusParameters& param) = 0;

		virtual const sDistortionParameters& getDistortionParameters() const = 0;
		virtual void setDistortionParameters(const sDistortionParameters& param) = 0;

		virtual const sEchoParameters& getEchoParameters() const = 0;
		virtual void setEchoParameters(const sEchoParameters& param) = 0;

		virtual const sFlangerParameters& getFlangerParameters() const = 0;
		virtual void setFlangerParameters(const sFlangerParameters& param) = 0;

		virtual const sFrequencyShiftParameters& getFrequencyShiftParameters() const = 0;
		virtual void setFrequencyShiftParameters(const sFrequencyShiftParameters& param) = 0;

		virtual const sVocalMorpherParameters& getVocalMorpherParameters() const = 0;
		virtual void setVocalMorpherParameters(const sVocalMorpherParameters& param) = 0;

		virtual const sPitchShifterParameters& getPitchShifterParameters() const = 0;
		virtual void setPitchShifterParameters(const sPitchShifterParameters& param) = 0;

		virtual const sRingModulatorParameters& getRingModulatorParameters() const = 0;
		virtual void setRingModulatorParameters(const sRingModulatorParameters& param) = 0;

		virtual const sAutowahParameters& getAutowahParameters() const = 0;
		virtual void setAutowahParameters(const sAutowahParameters& param) = 0;

		virtual const sCompressorParameters& getCompressorParameters() const = 0;
		virtual void setCompressorParameters(const sCompressorParameters& param) = 0;

		virtual const sEqualizerParameters& getEqualizerParameters() const = 0;
		virtual void setEqualizerParameters(const sEqualizerParameters& param) = 0;

		virtual float getMasterVolume() const = 0;
		virtual void setMasterVolume(const float& volume) = 0;

		virtual bool isIgnoringAttenuation() const = 0;
		virtual void ignoreAttenuation(const bool& ignore) = 0;

		virtual IFilter* getFilter() const = 0;
		virtual void attachFilter(IFilter* filter) = 0;
		virtual void removeFilter() = 0;

		virtual unsigned int getLastUpdated() const = 0;
		virtual bool isValid() const = 0;
	};
};

#endif //! IEFFECT_H