// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CAUDIOEFFECTS_H
#define CAUDIOEFFECTS_H

#include "../include/IAudioEffects.h"
#include "../Headers/cMutex.h"
#include "../Headers/cEFXFunctions.h"

#ifdef CAUDIO_EFX_ENABLED

namespace cAudio
{
	class cAudioEffects : public IAudioEffects
    {
    public:
		cAudioEffects();
        virtual ~cAudioEffects();

		virtual IEffect* createEffect();
		virtual IFilter* createFilter();

		virtual unsigned int getMaxEffectsSupported() const;
		virtual bool isEffectSupported(const EffectTypes& type) const;
		virtual bool isFilterSupported(const FilterTypes& type) const;

		cEFXFunctions* getEFXInterface();
		void checkEFXSupportDetails();

    private:
		cAudioMutex Mutex;
		cEFXFunctions EFXInterface;
		unsigned int MaxEffectsSupported;
		bool SupportedEffects[EET_COUNT];
		bool SupportedFilters[EFT_COUNT];
    };
};

#endif

#endif //! CAUDIOEFFECTS_H