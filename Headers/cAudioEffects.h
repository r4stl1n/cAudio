#ifndef CAUDIOEFFECTS_H
#define CAUDIOEFFECTS_H

#include "../include/IAudioEffects.h"
#include "../Headers/cMutex.h"
#include "../Headers/cEFXFunctions.h"

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

#endif //! CAUDIOEFFECTS_H