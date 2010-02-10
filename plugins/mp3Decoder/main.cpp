#include "cMP3DecoderFactory.h"
#include "IAudioPlugin.h"
#include "cAudioDefines.h"
#include "IAudioManager.h"
#include "ILogger.h"

//Will be called on initial install of the plugin, use this for any first time initialization.
extern "C" CAUDIO_API bool InstallPlugin(cAudio::ILogger* logger, const char* version)
{
	//This plugin has no first time initialization to do, so this is an easy function
	//First, double check that this copy of cAudio matches what we were compiled against
	if(strcmp(CAUDIO_VERSION, version) == 0)
	{
		return true;
	}
	return false;
}

//Must return a unique name that identifies this plugin.
extern "C" CAUDIO_API const char* GetPluginName()
{
	return "MP3Decoder";
}

//Will be called on when cAudio uninstalls this plugin, use this for any final cleanup.
extern "C" CAUDIO_API void UninstallPlugin()
{

}

//Will be called when an Audio Manager is created.  Use the passed in pointer to register any decoders or event handlers.
extern "C" CAUDIO_API void OnCreateAudioManager(cAudio::IAudioManager* manager)
{
	cMP3DecoderFactory* factory = new cMP3DecoderFactory();

	if(factory)
	{
		manager->registerAudioDecoder(factory, "mp3");
	}
}

extern "C" CAUDIO_API void OnDestroyAudioManager(cAudio::IAudioManager* manager)
{
	cAudio::IAudioDecoderFactory* factory = manager->getAudioDecoderFactory("mp3");
	manager->unRegisterAudioDecoder("mp3");

	if(factory)
	{
		delete factory;
	}
}
