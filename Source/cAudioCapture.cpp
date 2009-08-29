#include "../Headers/cAudioCapture.h"
#include <string.h>

namespace cAudio
{
	cAudioCapture::cAudioCapture() : Frequency(22050), Format(EAF_16BIT_MONO), InternalBufferSize(8192),
									Supported(false), Ready(false), Capturing(false), SampleSize(2), CaptureDevice(NULL)
	{

	}
	cAudioCapture::~cAudioCapture()
	{

	}

	bool cAudioCapture::checkCaptureExtension()
	{
		Mutex.lock();
		// Check for Capture Extension support
		ALCdevice* pDevice;
		ALCcontext* pContext;
		pContext = alcGetCurrentContext();
		pDevice = alcGetContextsDevice(pContext);
		Supported = ( alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE") == AL_TRUE );
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

			CaptureDevice = alcCaptureOpenDevice(NULL, Frequency, Format, InternalBufferSize / SampleSize);
			if(CaptureDevice)
			{
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
		}
		Mutex.unlock();
	}

	void cAudioCapture::shutdown()
	{
		Mutex.lock();
		shutdownOpenALDevice();
		Mutex.unlock();
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
					const int oldBufferSize = CaptureBuffer.size();
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
		if(outputBuffer && outputBufferSize > 0)
		{
			int sizeToCopy = (outputBufferSize >= CaptureBuffer.size()) ? CaptureBuffer.size() : outputBufferSize;
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

	bool cAudioCapture::initialize(unsigned int frequency, AudioFormats format, unsigned int internalBufferSize)
	{
		Mutex.lock();
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
};
