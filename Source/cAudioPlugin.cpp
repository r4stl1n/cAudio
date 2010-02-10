#include "../Headers/cAudioPlugin.h"

namespace cAudio
{

bool cAudioPlugin::installPlugin(ILogger* logger, const char* version)
{
	if(initFunc)
		return initFunc(logger, version);

	return false;
}

const char* cAudioPlugin::getPluginName()
{
	if(nameFunc)
		return nameFunc();

	return 0x0;
}

void cAudioPlugin::uninstallPlugin()
{
	if(uninstalledFunc)
		uninstalledFunc();
}

void cAudioPlugin::onCreateAudioManager(IAudioManager* manager)
{
	if(createAudioManagerFunc)
		createAudioManagerFunc(manager);
}

void cAudioPlugin::onCreateAudioCapture(IAudioCapture* capture)
{
	if(createAudioCaptureFunc)
		createAudioCaptureFunc(capture);
}

void cAudioPlugin::onDestroyAudioManager(IAudioManager* manager)
{
	if(destroyAudioManagerFunc)
		destroyAudioManagerFunc(manager);
}

void cAudioPlugin::onDestoryAudioCapture(IAudioCapture* capture)
{
	if(destroyAudioCaptureFunc)
		destroyAudioCaptureFunc(capture);
}

};