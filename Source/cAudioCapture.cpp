#include "../Headers/cAudioCapture.h"
#include "../Headers/cUtils.h"
#include "../Headers/cThread.h"
#include "../include/cAudioSleep.h"

#include <string.h>
#include <set>

namespace cAudio
{
	static bool RunAudioCaptureThread(false);

	//Note: OpenAL is threadsafe, so a mutex only needs to protect the class state
#ifdef CAUDIO_USE_INTERNAL_THREAD
	static cAudioMutex AudioCaptureObjectsMutex;
	static std::set<IAudioCapture*> AudioCaptureObjects;

	CAUDIO_DECLARE_THREAD_FUNCTION(AudioCaptureUpdateThread)
	{
		while(RunAudioCaptureThread)
		{
			AudioCaptureObjectsMutex.lock();
			std::set<IAudioCapture*>::iterator it;
			for ( it=AudioCaptureObjects.begin() ; it != AudioCaptureObjects.end(); it++ )
			{
				(*it)->updateCaptureBuffer();
			}
			AudioCaptureObjectsMutex.unlock();
			cAudioSleep(1);
		}
		return 0;
	}
#endif

	cAudioCapture::cAudioCapture() : Frequency(22050), Format(EAF_16BIT_MONO), InternalBufferSize(8192),
									SampleSize(2), Supported(false), Ready(false), Capturing(false), 
									CaptureDevice(NULL)
	{
		checkCaptureExtension();
		getAvailableDevices();
	}
	cAudioCapture::~cAudioCapture()
	{
		shutdown();
	}

	bool cAudioCapture::checkCaptureExtension()
	{
		Mutex.lock();
		// Check for Capture Extension support
		Supported = ( alcIsExtensionPresent(NULL, "ALC_EXT_CAPTURE") == AL_TRUE );
		Mutex.unlock();
		return Supported;
	}

	bool cAudioCapture::initOpenALDevice()
	{
		Mutex.lock();
		if(Supported)
		{
			if(CaptureDevice)
				shutdownOpenALDevice();
			if(DeviceName.empty())
				CaptureDevice = alcCaptureOpenDevice(NULL, Frequency, Format, InternalBufferSize / SampleSize);
			else
				CaptureDevice = alcCaptureOpenDevice(DeviceName.c_str(), Frequency, Format, InternalBufferSize / SampleSize);
			if(CaptureDevice)
			{
				DeviceName = alcGetString(CaptureDevice, ALC_CAPTURE_DEVICE_SPECIFIER);
				Ready = true;
				Mutex.unlock();
				return true;
			}
		}
		Mutex.unlock();
		return false;
	}

	void cAudioCapture::shutdownOpenALDevice()
	{
		Mutex.lock();
		if(Supported)
		{
			if(Capturing)
				stopCapture();

			if(CaptureDevice)
			{
				alcCaptureCloseDevice(CaptureDevice);
				CaptureDevice = NULL;
				Ready = false;
			}
			CaptureBuffer.clear();
		}
		Mutex.unlock();
	}

	void cAudioCapture::shutdown()
	{
		Mutex.lock();
		shutdownOpenALDevice();
		Mutex.unlock();
	}

	void cAudioCapture::getAvailableDevices()
	{
		// Get list of available Capture Devices
		Mutex.lock();
		if( alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE )
		{
			const char* deviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
			if (deviceList)
			{
				while(*deviceList)
				{
					std::string device(deviceList);
					AvailableDevices.push_back(device);
					deviceList += strlen(deviceList) + 1;
				}
			}

			// Get the name of the 'default' capture device
			DefaultDevice = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
		}
		Mutex.unlock();
	}

	const char* cAudioCapture::getAvailableDeviceName(unsigned int index)
	{
		Mutex.lock();
		if(!AvailableDevices.empty())
		{
			//Bounds check
			if( index > (AvailableDevices.size()-1) ) index = (AvailableDevices.size()-1);
			const char* deviceName = AvailableDevices[index].c_str();
			Mutex.unlock();
			return deviceName;
		}
		Mutex.unlock();
		return "";
	}

	unsigned int cAudioCapture::getAvailableDeviceCount()
	{
		Mutex.lock();
		unsigned int size = AvailableDevices.size();
		Mutex.unlock();
		return size;
	}

	const char* cAudioCapture::getDefaultDeviceName()
	{
		Mutex.lock();
		const char* deviceName = DefaultDevice.empty() ? "" : DefaultDevice.c_str();
		Mutex.unlock();
		return deviceName;
	}

	void cAudioCapture::updateCaptureBuffer(bool force)
	{
		Mutex.lock();
		if(Capturing && CaptureDevice && Ready)
		{
			int AvailableSamples = 0;
			alcGetIntegerv(CaptureDevice, ALC_CAPTURE_SAMPLES, 1, &AvailableSamples);
			const unsigned int availbuffersize = AvailableSamples * SampleSize;

			//If the samples in the OpenAL buffer are more than half of its max size, grab them
			if(availbuffersize > InternalBufferSize / 2 || force)
			{
				//Fixes a bug with the capture being forced, but no data being available
				if(availbuffersize > 0)
				{
					const unsigned int oldBufferSize = CaptureBuffer.size();
					CaptureBuffer.resize(oldBufferSize + availbuffersize, 0);
					alcCaptureSamples(CaptureDevice, &CaptureBuffer[oldBufferSize], AvailableSamples);
				}
			}
		}
		Mutex.unlock();
	}

	bool cAudioCapture::beginCapture()
	{
		Mutex.lock();
		if(!Capturing)
		{
			CaptureBuffer.clear();
			if(CaptureDevice && Ready)
			{
				alcCaptureStart(CaptureDevice);
				Capturing = true;
			}
			Mutex.unlock();
			return Capturing;
		}
		else
		{
			Mutex.unlock();
			return false;
		}
	}

	void cAudioCapture::stopCapture()
	{
		Mutex.lock();
		if(CaptureDevice && Ready)
		{
			alcCaptureStop(CaptureDevice);
			updateCaptureBuffer(true);
		}
		Capturing = false;
		Mutex.unlock();
	}

	unsigned int cAudioCapture::getCapturedAudio(void* outputBuffer, unsigned int outputBufferSize)
	{
		Mutex.lock();
		unsigned int internalBufferSize = CaptureBuffer.size();
		if(outputBuffer && outputBufferSize > 0 && internalBufferSize > 0)
		{
			int sizeToCopy = (outputBufferSize >= internalBufferSize) ? internalBufferSize : outputBufferSize;
			memcpy(outputBuffer, &CaptureBuffer[0], sizeToCopy);
			CaptureBuffer.erase(CaptureBuffer.begin(), CaptureBuffer.begin()+sizeToCopy);

			Mutex.unlock();
			return sizeToCopy;
		}
		Mutex.unlock();
		return 0;
	}

	unsigned int cAudioCapture::getCurrentCapturedAudioSize()
	{
		Mutex.lock();
		unsigned int size = CaptureBuffer.size();
		Mutex.unlock();
		return size;
	}

	bool cAudioCapture::setFrequency(unsigned int frequency)
	{
		Mutex.lock();
		Frequency = frequency;
		shutdownOpenALDevice();
		bool state = initOpenALDevice();
		Mutex.unlock();
		return state;
	}

	bool cAudioCapture::setFormat(AudioFormats format)
	{
		Mutex.lock();
		Format = format;
		if(Format == EAF_8BIT_MONO)
			SampleSize = 1;
		else if(Format == EAF_8BIT_STEREO)
			SampleSize = 2;
		else if(Format == EAF_16BIT_MONO)
			SampleSize = 2;
		else
			SampleSize = 4;

		shutdownOpenALDevice();
		bool state = initOpenALDevice();
		Mutex.unlock();
		return state;
	}

	bool cAudioCapture::setInternalBufferSize(unsigned int internalBufferSize)
	{
		Mutex.lock();
		InternalBufferSize = internalBufferSize;

		shutdownOpenALDevice();
		bool state = initOpenALDevice();
		Mutex.unlock();
		return state;
	}

	bool cAudioCapture::setDevice(const char* deviceName)
	{
		Mutex.lock();
		DeviceName = safeCStr(deviceName);

		shutdownOpenALDevice();
		bool state = initOpenALDevice();
		Mutex.unlock();
		return state;
	}

	bool cAudioCapture::initialize(const char* deviceName, unsigned int frequency, AudioFormats format, unsigned int internalBufferSize)
	{
		Mutex.lock();
		DeviceName = safeCStr(deviceName);
		Frequency = frequency;
		InternalBufferSize = internalBufferSize;

		Format = format;
		if(Format == EAF_8BIT_MONO)
			SampleSize = 1;
		else if(Format == EAF_8BIT_STEREO)
			SampleSize = 2;
		else if(Format == EAF_16BIT_MONO)
			SampleSize = 2;
		else
			SampleSize = 4;

		shutdownOpenALDevice();
		bool state = initOpenALDevice();
		Mutex.unlock();
		return state;
	}

	CAUDIO_API IAudioCapture* createAudioCapture(bool initializeDefault)
	{
		cAudioCapture* capture = new cAudioCapture;
		if(capture)
		{
			if(initializeDefault)
				capture->initialize();

#ifdef CAUDIO_USE_INTERNAL_THREAD
			AudioCaptureObjectsMutex.lock();
			AudioCaptureObjects.insert(capture);

			//First time launch of thread
			if(!RunAudioCaptureThread && AudioCaptureObjects.size() > 0)
				RunAudioCaptureThread = (cAudioThread::SpawnThread(AudioCaptureUpdateThread, NULL) == 0);
			AudioCaptureObjectsMutex.unlock();
#endif
		}
		return capture;
	}

	CAUDIO_API void destroyAudioCapture(IAudioCapture* capture)
	{
		if(capture)
		{
#ifdef CAUDIO_USE_INTERNAL_THREAD
			AudioCaptureObjectsMutex.lock();
			AudioCaptureObjects.erase(capture);

			//Kill the thread if there are no objects to process anymore
			if(RunAudioCaptureThread && AudioCaptureObjects.empty())
				RunAudioCaptureThread = false;
			AudioCaptureObjectsMutex.unlock();
#endif
			delete capture;
			capture = NULL;
		}
	}

	CAUDIO_API bool isAudioCaptureThreadRunning()
	{
		return RunAudioCaptureThread;
	}
};
