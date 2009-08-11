#ifndef IAUDIOCAPTURE_H
#define IAUDIOCAPTURE_H

#include "eAudioFormats.h"

namespace cAudio
{
	class IAudioCapture
	{
	public:
		//! Initializes the capture device to the selected settings
		/** Note that calling this will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		\param frequency: Frequency that the captured audio will be at
		\param format: Format of the captured audio
		\param internalBufferSize: Size of the internal OpenAL buffer used to store the captured audio until the next IAudioManager::update() in bytes
		\return True on success, False if the capture device failed to initialize.
		*/
		virtual bool initialize(unsigned int frequency = 22050, AudioFormats format = EAF_16BIT_MONO, unsigned int internalBufferSize = 8192) = 0;
		//! Returns true if the current OpenAL implementation supports capturing audio
		virtual bool isSupported() = 0;
		//! Returns true if the capture device is ready to be used.  False may indicate an error with the current settings. 
		virtual bool isReady() = 0;

		//! Returns the frequency that the captured audio will be at
		virtual unsigned int getFrequency() = 0;
		//! Returns the format of the captured audio
		virtual AudioFormats getFormat() = 0;
		//! Returns the internal OpenAL buffer size in bytes
		virtual unsigned int getInternalBufferSize() = 0;
		//! Returns the size of a "sample" of audio data.  Useful for making sure you grab audio data at sample boundaries
		virtual unsigned int getSampleSize() = 0;

		//! Sets the frequency that the captured audio will be at.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setFrequency(unsigned int frequency) = 0;
		//! Sets the format that the captured audio will be at.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setFormat(AudioFormats format) = 0;
		//! Sets the internal buffer size that OpenAL will use to store captured audio between calls to IAudioManager::update() in bytes.  Will cause the capture device to be reinitialized.  Calling while in use may lead to a loss of audio data.
		/** \return True on success, False if the capture device failed to initialize. */
		virtual bool setInternalBufferSize(unsigned int internalBufferSize) = 0;

		//! Starts capturing audio data to an internal buffer.  Will clear any old data in the buffer.
		/** \return True if capture was successfully started. */
		virtual bool beginCapture() = 0;
		//! Stops capturing audio data to an internal buffer.
		virtual void stopCapture() = 0;
		//! Allows access to the audio data in the internal capture buffer
		/** Can be called at any time to retrieve recorded audio.  It is recommended that you call it every so often to prevent the internal buffer from growing too large.
		Once successfully retrieved, the captured audio will be deleted from the internal buffer.
		\param outputBuffer: Pointer to an output array to copy audio data to.
		\param outputBufferSize: Size of the output array in bytes
		*/
		virtual void getCapturedAudio(void* outputBuffer, unsigned int outputBufferSize) = 0;

		//! Returns the current size of the internal audio buffer in bytes
		virtual unsigned int getCurrentCapturedAudioSize() = 0;
	protected:
	};
};

#endif //! IAUDIOCAPTURE_H