#ifndef IAUDIOEFFECTS_H
#define IAUDIOEFFECTS_H

#include "IEffect.h"
#include "IFilter.h"
#include "cAudioDefines.h"

namespace cAudio
{
    class IAudioEffects
    {
    public:
		IAudioEffects() {}
        virtual ~IAudioEffects() {}

		virtual IEffect* createEffect() = 0;
		virtual IFilter* createFilter() = 0;

		virtual unsigned int getMaxEffectsSupported() const = 0;
		virtual bool isEffectSupported(const EffectTypes& type) const = 0;
		virtual bool isFilterSupported(const FilterTypes& type) const = 0;

    protected:
    private:
    };
};

#endif //! IAUDIOEFFECTS_H