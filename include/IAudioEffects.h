// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

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

    protected:
    private:
    };
};

#endif //! IAUDIOEFFECTS_H