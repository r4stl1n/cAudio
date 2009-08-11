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
		// Check for Capture Extension support
		ALCdevice* pDevice;
		ALCcontext* pContext;
		pContext = alcGetCurrentContext();
		pDevice = alcGetContextsDevice(pContext);
		Supported = ( alcIsExtensionPresent(pDevice, "ALC_EXT_CAPTURE") == AL_TRUE );
		return Supported;
	}

	bool cAudioCapture::initOpenALDevice()
	{
		if(Supported)
		{
			if(CaptureDevice)
				shutdownOpenALDevice();

			CaptureDevice = alcCaptureOpenDevice(NULL, Frequency, Format, InternalBufferSize / SampleSize);
			if(CaptureDevice)
			{
				Ready = true;
				return true;
			}
		}
		return false;
	}

	void cAudioCapture::shutdownOpenALDevice()
	{
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
	}

	void cAudioCapture::shutdown()
	{
		shutdownOpenALDevice();
	}

	void cAudioCapture::updateCaptureBuffer(bool force)
	{
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
	}

	bool cAudioCapture::beginCapture()
	{
		if(!Capturing)
		{
			CaptureBuffer.clear();
			if(CaptureDevice && Ready)
			{
				alcCaptureStart(CaptureDevice);
				Capturing = true;
			}
			return Capturing;
		}
		else
			return false;
	}

	void cAudioCapture::stopCapture()
	{
		if(CaptureDevice && Ready)
		{
			alcCaptureStop(CaptureDevice);
			updateCaptureBuffer(true);
		}
		Capturing = false;
	}

	void cAudioCapture::getCapturedAudio(void* outputBuffer, unsigned int outputBufferSize)
	{
		if(outputBuffer && outputBufferSize > 0)
		{
			int sizeToCopy = (outputBufferSize >= CaptureBuffer.size()) ? CaptureBuffer.size() : outputBufferSize;
			memcpy(outputBuffer, &CaptureBuffer[0], sizeToCopy);
			CaptureBuffer.erase(CaptureBuffer.begin(), CaptureBuffer.begin()+sizeToCopy);
		}
	}

	unsigned int cAudioCapture::getCurrentCapturedAudioSize()
	{
		return CaptureBuffer.size();
	}

	bool cAudioCapture::setFrequency(unsigned int frequency)
	{
		Frequency = frequency;
		shutdownOpenALDevice();
		return initOpenALDevice();
	}

	bool cAudioCapture::setFormat(AudioFormats format)
	{
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
		return initOpenALDevice();
	}

	bool cAudioCapture::setInternalBufferSize(unsigned int internalBufferSize)
	{
		InternalBufferSize = internalBufferSize;

		shutdownOpenALDevice();
		return initOpenALDevice();
	}

	bool cAudioCapture::initialize(unsigned int frequency, AudioFormats format, unsigned int internalBufferSize)
	{
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
		return initOpenALDevice();
	}
};
