#ifndef CPLUGINMANAGER_H_INCLUDED
#define CPLUGINMANAGER_H_INCLUDED

#include <string>
#include <map>
#include <vector>

#include "../include/IPluginManager.h"
#include "../include/IAudioPlugin.h"
#include "../Headers/cMutex.h"

#ifdef CAUDIO_PLATFORM_WIN
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#ifdef CAUDIO_PLATFORM_MAC
#	include "macUtils.h"
#   include <dlfcn.h>
#endif

#ifdef CAUDIO_PLATFORM_LINUX
#	include <dlfcn.h>
#endif

#ifdef CAUDIO_PLATFORM_WIN
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#endif

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#ifdef CAUDIO_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

#ifdef CAUDIO_PLATFORM_MAC
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) mac_loadDylib( a )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

namespace cAudio
{
	class cPluginManager : public IPluginManager
	{
	public:
		cPluginManager();
		~cPluginManager();

		static cPluginManager* Instance()
		{
			static cPluginManager theInstance;
			return &theInstance;
		}

		virtual bool installPlugin(IAudioPlugin* plugin, const char* name);
		virtual bool installPlugin(const char* filename, const char* name);

		virtual bool checkForPlugin(const char* name);
		virtual IAudioPlugin* getPlugin(const char* name);
		virtual unsigned int getPluginCount();
		std::vector<IAudioPlugin*> getPluginList();

		virtual void uninstallPlugin(IAudioPlugin* plugin);
		virtual void uninstallPlugin(const char* name);

		void autoLoadPlugins();
	protected:
		std::map<std::string, IAudioPlugin*> RegisteredPlugins;
		std::map<IAudioPlugin*, DYNLIB_HANDLE> DynamicallyLoadedPlugins;
	};
};

#endif //! CPLUGINMANAGER_H_INCLUDED