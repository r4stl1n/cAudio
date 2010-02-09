#ifndef CAUDIOPLUGIN_H_INCLUDED
#define CAUDIOPLUGIN_H_INCLUDED

#include "../include/IAudioPlugin.h"

namespace cAudio
{
	typedef bool (*pluginInstallFunc)();
	typedef const char* (*pluginNameFunc)();
	typedef void (*pluginUninstallFunc)();

	typedef void (*pluginOnCreateAudioManager)(IAudioManager*);
	typedef void (*pluginOnCreateAudioCapture)(IAudioCapture*);

	typedef void (*pluginOnDestroyAudioManager)(IAudioManager*);
	typedef void (*pluginOnDestroyAudioCapture)(IAudioCapture*);

	//Simply acts as a shim between dll functions and IAudioPlugin
	class cAudioPlugin : public IAudioPlugin
	{
	public:
		cAudioPlugin()
		{
			initFunc = 0x0;
			nameFunc = 0x0;
			uninstalledFunc = 0x0;
			createAudioManagerFunc = 0x0;
			createAudioCaptureFunc = 0x0;
			destroyAudioManagerFunc = 0x0;
			destroyAudioCaptureFunc = 0x0;
		}
		~cAudioPlugin() { }

		virtual bool installPlugin();
		virtual const char* getPluginName();
		virtual void uninstallPlugin();

		virtual void onCreateAudioManager(IAudioManager* manager);
		virtual void onCreateAudioCapture(IAudioCapture* capture);

		virtual void onDestroyAudioManager(IAudioManager* manager);
		virtual void onDestoryAudioCapture(IAudioCapture* capture);

		pluginInstallFunc initFunc;
		pluginNameFunc nameFunc;
		pluginUninstallFunc uninstalledFunc;

		pluginOnCreateAudioManager createAudioManagerFunc;
		pluginOnCreateAudioCapture createAudioCaptureFunc;

		pluginOnDestroyAudioManager destroyAudioManagerFunc;
		pluginOnDestroyAudioCapture destroyAudioCaptureFunc;
	};
};

#endif //! CAUDIOPLUGIN_H_INCLUDED