#ifndef CEFXFUNCTIONS_H
#define CEFXFUNCTIONS_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/efx-creative.h>

#include "../Headers/cMutex.h"

// EFX Extension function pointer variables
namespace cAudio
{
	struct cEFXFunctions
	{
		cEFXFunctions::cEFXFunctions()
		{
			alGenEffects = NULL;
			alDeleteEffects = NULL;
			alIsEffect = NULL;
			alEffecti = NULL;
			alEffectiv = NULL;
			alEffectf = NULL;
			alEffectfv = NULL;
			alGetEffecti = NULL;
			alGetEffectiv = NULL;
			alGetEffectf = NULL;
			alGetEffectfv = NULL;

			alGenFilters = NULL;
			alDeleteFilters = NULL;
			alIsFilter = NULL;
			alFilteri = NULL;
			alFilteriv = NULL;
			alFilterf = NULL;
			alFilterfv = NULL;
			alGetFilteri = NULL;
			alGetFilteriv = NULL;
			alGetFilterf = NULL;
			alGetFilterfv = NULL;

			alGenAuxiliaryEffectSlots = NULL;
			alDeleteAuxiliaryEffectSlots = NULL;
			alIsAuxiliaryEffectSlot = NULL;
			alAuxiliaryEffectSloti = NULL;
			alAuxiliaryEffectSlotiv = NULL;
			alAuxiliaryEffectSlotf = NULL;
			alAuxiliaryEffectSlotfv = NULL;
			alGetAuxiliaryEffectSloti = NULL;
			alGetAuxiliaryEffectSlotiv = NULL;
			alGetAuxiliaryEffectSlotf = NULL;
			alGetAuxiliaryEffectSlotfv = NULL;

			Supported = false;
		}

		// Effect objects
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;
		LPALEFFECTI alEffecti;
		LPALEFFECTIV alEffectiv;
		LPALEFFECTF alEffectf;
		LPALEFFECTFV alEffectfv;
		LPALGETEFFECTI alGetEffecti;
		LPALGETEFFECTIV alGetEffectiv;
		LPALGETEFFECTF alGetEffectf;
		LPALGETEFFECTFV alGetEffectfv;

		// Filter objects
		LPALGENFILTERS alGenFilters;
		LPALDELETEFILTERS alDeleteFilters;
		LPALISFILTER alIsFilter;
		LPALFILTERI alFilteri;
		LPALFILTERIV alFilteriv;
		LPALFILTERF alFilterf;
		LPALFILTERFV alFilterfv;
		LPALGETFILTERI alGetFilteri;
		LPALGETFILTERIV alGetFilteriv;
		LPALGETFILTERF alGetFilterf;
		LPALGETFILTERFV alGetFilterfv;

		// Auxiliary slot object
		LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
		LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
		LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
		LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
		LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
		LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
		LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
		LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
		LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
		LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
		LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

		cAudioMutex Mutex;

		bool Supported;

		bool CheckEFXSupport(ALCdevice* device)
		{
			Supported = false;
			if (alcIsExtensionPresent(device, "ALC_EXT_EFX"))
			{
				// Get function pointers
				alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
				alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
				alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
				alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
				alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
				alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
				alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
				alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
				alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
				alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
				alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
				alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
				alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
				alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
				alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
				alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
				alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
				alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
				alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
				alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
				alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
				alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
				alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
				alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
				alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
				alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
				alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
				alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
				alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
				alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
				alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
				alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
				alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

				if (alGenEffects &&	alDeleteEffects && alIsEffect && alEffecti && alEffectiv &&	alEffectf &&
					alEffectfv && alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&	alGenFilters &&
					alDeleteFilters && alIsFilter && alFilteri && alFilteriv &&	alFilterf && alFilterfv &&
					alGetFilteri &&	alGetFilteriv && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots &&
					alDeleteAuxiliaryEffectSlots &&	alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
					alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
					alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf &&
					alGetAuxiliaryEffectSlotfv)
					Supported = true;
			}

			return Supported;
		}
	};
};

#endif //! CEFXFUNCTIONS_H