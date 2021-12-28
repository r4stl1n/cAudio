// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cAudio.h"
#include "../Headers/cAudioManager.h"
#include "../Headers/cAudioCapture.h"

#if CAUDIO_COMPILE_WITH_OGG_DECODER == 1
#include "../Headers/cOggAudioDecoderFactory.h"
#endif

#if CAUDIO_COMPILE_WITH_WAV_DECODER == 1
#include "../Headers/cWavAudioDecoderFactory.h"
#endif

#include "../Headers/cRawAudioDecoderFactory.h"

#if CAUDIO_COMPILE_WITH_FILE_SOURCE == 1
#include "../Headers/cFileSourceFactory.h"
#include "../Headers/cFileSource.h"
#endif

#include "../Headers/cPluginManager.h"
#include "../Headers/cLogger.h"
#include "../Headers/cConsoleLogReceiver.h"
#include "../Headers/cFileLogReceiver.h"
#include "../Headers/cOpenALAudioDeviceList.h"

namespace cAudio
{

//---------------------------------------------------------------------------------------
// Logger section
//---------------------------------------------------------------------------------------

#if CAUDIO_COMPILE_WITH_CONSOLE_LOG_RECEIVER == 1
	static cConsoleLogReceiver ConsoleLog;
#endif

#if CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER == 1
        static cFileLogReceiver *FileLog;
#endif

  CAUDIO_API ILogger* getLogger()
  {
    static cLogger* Logger = NULL;
    if(!Logger)
      {
	Logger = new cLogger;
#if CAUDIO_COMPILE_WITH_CONSOLE_LOG_RECEIVER == 1
	Logger->registerLogReceiver(&ConsoleLog, "Console");
#endif
#if CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER == 1
	Logger->registerLogReceiver(FileLog,"File");
#endif
      }
    return Logger;
  }
  
//---------------------------------------------------------------------------------------
// Audio manager section
//---------------------------------------------------------------------------------------

#if CAUDIO_COMPILE_WITH_OGG_DECODER == 1
	static cOggAudioDecoderFactory OggDecoderFactory;
#endif
#if CAUDIO_COMPILE_WITH_WAV_DECODER == 1
	static cWavAudioDecoderFactory WavDecoderFactory;
#endif

	static cRawAudioDecoderFactory RawDecoderFactory;

#if CAUDIO_COMPILE_WITH_FILE_SOURCE == 1
	static cFileSourceFactory FileSourceFactory;
#endif
  CAUDIO_API IAudioManager* createAudioManager(bool initializeDefault, const char *lFilePath)
	{
		cAudioManager* manager = CAUDIO_NEW cAudioManager;
#if CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER == 1
		if(FileLog == NULL)

         		FileLog = new cFileLogReceiver(lFilePath);
#endif
		if(manager)
		{
			if(initializeDefault) 
				manager->initialize();

#if CAUDIO_COMPILE_WITH_OGG_DECODER == 1
			manager->registerAudioDecoder(&OggDecoderFactory, "ogg");
#endif
#if CAUDIO_COMPILE_WITH_WAV_DECODER == 1
			manager->registerAudioDecoder(&WavDecoderFactory, "wav");
#endif

			manager->registerAudioDecoder(&RawDecoderFactory, "raw");

#if CAUDIO_COMPILE_WITH_FILE_SOURCE == 1
			manager->registerDataSource(&FileSourceFactory, "FileSystem", 0);
#endif

#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT
			cAudioVector<IAudioPlugin*>::Type plugins = cPluginManager::Instance()->getPluginList();
			for(unsigned int i = 0; i < plugins.size(); ++i)
			{
				plugins[i]->onCreateAudioManager(manager);
			}
#endif
		}
		return manager;
	}

	CAUDIO_API void destroyAudioManager(IAudioManager* manager)
	{
#if CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER == 1
	  if(FileLog != NULL)
         	  delete FileLog;
#endif
		if(manager)
		{
#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT
			cAudioVector<IAudioPlugin*>::Type plugins = cPluginManager::Instance()->getPluginList();
			for(unsigned int i = 0; i < plugins.size(); ++i)
			{
				plugins[i]->onDestroyAudioManager(manager);
			}
#endif

			manager->shutDown();

			CAUDIO_DELETE manager;
			manager = NULL;
		}
	}

	//---------------------------------------------------------------------------------------
	// Audio capture section
	//---------------------------------------------------------------------------------------

	CAUDIO_API IAudioCapture* createAudioCapture(bool initializeDefault)
	{
		cAudioCapture* capture = CAUDIO_NEW cAudioCapture;
		if(capture)
		{
			if(initializeDefault)
				capture->initialize();			

#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT
			cAudioVector<IAudioPlugin*>::Type plugins = cPluginManager::Instance()->getPluginList();
			for(unsigned int i = 0; i < plugins.size(); ++i)
			{
				plugins[i]->onCreateAudioCapture(capture);
			}
#endif
		}
		return capture;
	}

	CAUDIO_API void destroyAudioCapture(IAudioCapture* capture)
	{
		if(capture)
		{
#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT
			cAudioVector<IAudioPlugin*>::Type plugins = cPluginManager::Instance()->getPluginList();
			for(unsigned int i = 0; i < plugins.size(); ++i)
			{
				plugins[i]->onDestoryAudioCapture(capture);
			}
#endif
			CAUDIO_DELETE capture;
			capture = NULL;
		}
	}

	//---------------------------------------------------------------------------------------
	// IAudioDeviceList section
	//---------------------------------------------------------------------------------------
	CAUDIO_API IAudioDeviceList* createAudioDeviceList(IDeviceType deviceType)
	{
		return CAUDIO_NEW cOpenALAudioDeviceList(deviceType);
	}
}
