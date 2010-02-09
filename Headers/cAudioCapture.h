// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef CAUDIOCAPTURE_H
#define CAUDIOCAPTURE_H

#include "../include/IAudioCapture.h"
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "../Headers/cMutex.h"
#include <string>

namespace cAudio
{
	class cAudioCapture : public IAudioCapture
	{
	public:
		cAudioCapture();
		~cAudioCapture();

		//! Checks to see if capturing audio is supported by OpenAL
		bool checkCaptureExtension();

		//! Initializes the capture device to the selected settings
		/** Note that calling this will cause the capture device to be reinitialized.  Calling while in use will clear the internal audio buffer.
		\param deviceName: Name of the audio device to capture audio from, pass NULL to specify the default one
		\param frequency: Frequency that the captured audio will be at in hz
		\param format: Format of the captured audio
		\param internalBufferSize: Size of the internal OpenAL buffer used to store the captured audio between calls to getCapturedAudio() in bytes.
		\return True on success, False if the capture device failed to initialize.
		*/
		virtual bool initialize(const char* deviceName = 0x0, unsigned int frequency = 22050, AudioFormats format = EAF_16BIT_MONO, unsigned int internalBufferSize = 8192);
		//! Returns true if the current OpenAL implementation supports capturing audio
		virtual bool isSupported() { return Supported; }
		//! Returns true if the capture device is ready to be used.  False may indicate an error with the current settings. 
		virtual bool isReady() { return Ready; }
		//! Grabs samples from the OpenAL buffer into the capture buffer.  Should be run once every audio frame.
		virtual void updateCaptureBuffer(bool force = false);
		//! Shuts down the capture device
		virtual void shutdown();

		//! Returns the name of an available capturing device.
		/** \param index: Specify which name to retrieve ( Range: 0 to getAvailableDeviceCount()-1 ) */
		virtual const char* getAvailableDeviceName(unsigned int index);
		//! Returns the number of capture devices available for use.
		virtual unsigned int getAvailableDeviceCount();
		//! Returns the name of the default system capturing device.
		virtual const char* getDefaultDeviceName();

		//! Returns the name of the audio device being used to capture audio
		virtual const char* getDeviceName() { return DeviceName.c_str(); }
		//! Returns the frequency that the captured audio will be at
		virtual unsigned int getFrequency() { return Frequency; }
		//! Returns the format of the captured audio
		virtual AudioFormats getFormat() { return Format; }
		//! Returns the internal OpenAL buffer size in bytes
		virtual unsigned int getInternalBufferSize() { return InternalBufferSize; }
		//! Returns the size of a "sample" of audio data.  Useful for making sure you grab audio data at sample boundaries
		virtual unsigned int getSampleSize() { return SampleSize; }

		//! Sets the audio device .  Will cause the capture device to be reinitialized.  Calling while in use will clear the internal audio buffer.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setDevice(const char* deviceName);
		//! Sets the frequency that the captured audio will be at.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setFrequency(unsigned int frequency);
		//! Sets the format that the captured audio will be at.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setFormat(AudioFormats format);
		//! Sets the internal buffer size that OpenAL will use to store captured audio between calls to IAudioManager::update() in bytes.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setInternalBufferSize(unsigned int internalBufferSize);

		//! Starts capturing audio data to an internal buffer.   Will clear any old data in the buffer.
		/** \return True if capture was successfully started. */
		virtual bool beginCapture();
		//! Stops capturing audio data to an internal buffer.  Will copy any remaining audio data to the capture buffer.
		virtual void stopCapture();
		//! Allows access to the audio data in the internal capture buffer
		/** Can be called at any time to retrieve recorded audio.  It is recommended that you call it every so often to prevent the internal buffer from growing too large.
		Once successfully retrieved, the captured audio will be deleted from the internal buffer.
		\param outputBuffer: Pointer to an output array to copy audio data to.
		\param outputBufferSize: Size of the output array in bytes
		*/
		virtual unsigned int getCapturedAudio(void* outputBuffer, unsigned int outputBufferSize);

		//! Returns the current size of the internal audio buffer in bytes
		virtual unsigned int getCurrentCapturedAudioSize();

		//! Grabs a list of available devices, as well as the default system one
		void getAvailableDevices();
	protected:
		//Mutex for thread syncronization
		cAudioMutex Mutex;

		bool initOpenALDevice();
		void shutdownOpenALDevice();

		unsigned int Frequency;
		AudioFormats Format;
		unsigned int InternalBufferSize;
		int SampleSize;

		std::vector<char> CaptureBuffer;
		std::vector<std::string> AvailableDevices;
		std::string DefaultDevice;

		bool Supported;
		bool Ready;
		bool Capturing;

		std::string DeviceName;
		ALCdevice* CaptureDevice;

		//! Check for OpenAL errors
		bool checkError();
		//Converts our audio format enum to OpenAL's
		ALenum convertAudioFormatEnum(AudioFormats format);
	};
};

#endif //! CAUDIOCAPTURE_H