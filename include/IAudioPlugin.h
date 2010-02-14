#ifndef IAUDIOPLUGIN_H_INCLUDED
#define IAUDIOPLUGIN_H_INCLUDED

#include "IRefCounted.h"
#include "IAudioManager.h"
#include "IAudioCapture.h"
#include "ILogger.h"

namespace cAudio
{
	class IAudioPlugin : public IRefCounted
	{
	public:
		IAudioPlugin() { }
		~IAudioPlugin() { }

		virtual bool installPlugin(ILogger* logger) = 0;
		virtual const char* getPluginName() = 0;
		virtual void uninstallPlugin() = 0;

		virtual void onCreateAudioManager(IAudioManager* manager) { }
		virtual void onCreateAudioCapture(IAudioCapture* capture) { }

		virtual void onDestroyAudioManager(IAudioManager* manager) { }
		virtual void onDestoryAudioCapture(IAudioCapture* capture) { }
	};
};

#endif //! IAUDIOPLUGIN_H_INCLUDED