#ifndef IPLUGINMANAGER_H_INCLUDED
#define IPLUGINMANAGER_H_INCLUDED

#include "IAudioPlugin.h"

namespace cAudio
{
	class IPluginManager
	{
	public:
		IPluginManager() { }
		~IPluginManager() { }

		virtual bool installPlugin(IAudioPlugin* plugin, const char* name = NULL) = 0;
		virtual bool installPlugin(const char* filename, const char* name = NULL) = 0;

		virtual bool checkForPlugin(const char* name) = 0;
		virtual IAudioPlugin* getPlugin(const char* name) = 0;
		virtual unsigned int getPluginCount() = 0;

		virtual void uninstallPlugin(IAudioPlugin* plugin) = 0;
		virtual void uninstallPlugin(const char* name) = 0;
	};
};

#endif //! IPLUGINMANAGER_H_INCLUDED