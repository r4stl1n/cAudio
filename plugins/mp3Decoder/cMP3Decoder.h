#ifndef CMP3DECODER_H
#define CMP3DECODER_H

#include "IAudioDecoder.h"

using namespace cAudio;

class cMP3Decoder : public IAudioDecoder
{
	public:
		cMP3Decoder(IDataSource* stream);
		~cMP3Decoder();

		//!Returns the format of the audio data
		virtual AudioFormats getFormat();

		//!Returns the frequency of the audio data
		virtual int getFrequency();

		//!Returns whether seeking is supported
		virtual bool isSeekingSupported();

		//!Returns whether the stream is valid for this codec
		virtual bool isValid();

		//!Reads a section of data out of the audio stream
		virtual int readAudioData(void* output, int amount);

		//!Sets the position to read data out of
		virtual bool setPosition(int position, bool relative);

		//!If seeking is supported, will seek the stream to seconds
		virtual bool seek(float seconds, bool relative);
};

#endif //! CMP3DECODER_H