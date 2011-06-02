// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "../Include/cAudio.h"
#include "../Headers/cAudioManager.h"
#include "../Headers/cAudioCapture.h"
#include "../Headers/cOggAudioDecoderFactory.h"
#include "../Headers/cWavAudioDecoderFactory.h"
#include "../Headers/cRawAudioDecoderFactory.h"
#include "../Headers/cFileSourceFactory.h"
#include "../Headers/cFileSource.h"
#include "../Headers/cPluginManager.h"

namespace cAudio
{
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

	CAUDIO_API IAudioManager* createAudioManager(bool initializeDefault)
	{
		cAudioManager* manager = CAUDIO_NEW cAudioManager;
		if(manager)
		{
			if(initializeDefault)
				manager->initialize();

			manager->getAvailableDevices();

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
		if(manager)
		{
#ifdef CAUDIO_COMPILE_WITH_PLUGIN_SUPPORT
			cAudioVector<IAudioPlugin*>::Type plugins = cPluginManager::Instance()->getPluginList();
			for(unsigned int i = 0; i < plugins.size(); ++i)
			{
				plugins[i]->onDestroyAudioManager(manager);
			}
#endif

			manager->unRegisterAllAudioDecoders();
			manager->unRegisterAllDataSources();
			manager->unRegisterAllEventHandlers();
			manager->shutDown();

			CAUDIO_DELETE manager;
			manager = NULL;
		}
	}

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
}
