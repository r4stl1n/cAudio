/*	cAudio.h -- interface of the 'cAudio Engine'

	Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.

*/

#ifndef CAUDIO_H
#define CAUDIO_H

#include "cAudioDefines.h"
#include "cAudioPlatform.h"
#include "cAudioSleep.h"
#include "EAudioFormats.h"
#include "IAudioSource.h"
#include "IAudioCapture.h"
#include "IAudioDecoder.h"
#include "IAudioDecoderFactory.h"
#include "IAudioEffects.h"
#include "IAudioManager.h"
#include "IDataSource.h"
#include "IEffect.h"
#include "IEffectParameters.h"
#include "IFilter.h"
#include "IListener.h"
#include "ILogger.h"
#include "ILogReceiver.h"
#include "IPluginManager.h"
#include "IRefCounted.h"

#endif