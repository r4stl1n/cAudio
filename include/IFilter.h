// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IFILTER_H
#define IFILTER_H

#include "IRefCounted.h"

namespace cAudio
{
	enum FilterTypes
	{
		EFT_NULL,
		EFT_LOWPASS,
		EFT_HIGHPASS,
		EFT_BANDPASS,
		EFT_COUNT
	};

	class IFilter : public IRefCounted
	{
	public:
		IFilter() {  }
		virtual ~IFilter() {  }

		virtual const FilterTypes& getType() const = 0;
		virtual void setType(const FilterTypes& type) = 0;

		virtual float getVolume() const = 0;
		virtual void setVolume(const float& volume) = 0;

		virtual float getLowFrequencyVolume() const = 0;
		virtual void setLowFrequencyVolume(const float& volumeLF) = 0;

		virtual float getHighFrequencyVolume() const = 0;
		virtual void setHighFrequencyVolume(const float& volumeHF) = 0;

		virtual unsigned int getLastUpdated() const = 0;
		virtual bool isValid() const = 0;
	};
};

#endif //! IFILTER_H