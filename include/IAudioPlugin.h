#ifndef IAUDIOPLUGIN_H_INCLUDED
#define IAUDIOPLUGIN_H_INCLUDED

namespace cAudio
{
	class IAudioManager;
	class IAudioCapture;
	class ILogger;

	//Class that abstracts a particular plugin implementation
	//Plugins must have the following functions:
	/*
	//Will be called on initial install of the plugin, use this for any first time initialization.  A reference to the logger is passed in for convenience.
	bool installPlugin(ILogger* logger);

	//Must return a unique name that identifies this plugin.
	const char* getPluginName();

	//Will be called on when cAudio uninstalls this plugin, use this for any final cleanup.
	void uninstallPlugin();

	//Optional Functions, implement one if you want to support that feature of the library

	//Will be called when an Audio Manager is created.  Use the passed in pointer to register any decoders or event handlers.
	void onCreateAudioManager(IAudioManager* manager);

	//Will be called when an Audio Capture Device is created.  Use the passed in pointer to register any event handlers.
	void onCreateAudioCapture(IAudioCapture* capture);

	//Will be called when an Audio Manager is destroyed.  Use the passed in pointer to remove any decoders or event handlers.
	void onDestroyAudioManager(IAudioManager* manager);

	//Will be called when an Audio Capture Device is destroyed.  Use the passed in pointer to remove any event handlers.
	void onDestroyAudioCapture(IAudioCapture* capture);

	*/
	class IAudioPlugin
	{
	public:
		IAudioPlugin() { }
		~IAudioPlugin() { }

		virtual bool installPlugin(ILogger* logger) = 0;
		virtual const char* getPluginName() = 0;
		virtual void uninstallPlugin() = 0;

		virtual void onCreateAudioManager(IAudioManager* manager) = 0;
		virtual void onCreateAudioCapture(IAudioCapture* capture) = 0;

		virtual void onDestroyAudioManager(IAudioManager* manager) = 0;
		virtual void onDestoryAudioCapture(IAudioCapture* capture) = 0;
	};
};

#endif //! IAUDIOPLUGIN_H_INCLUDED