#ifndef COGGDECODER_H_INCLUDED
#define COGGDECODER_H_INCLUDED

#include "../include/IAudioDecoder.h"
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <iostream>

namespace cAudio
{

    class cOggDecoder : public IAudioDecoder
    {
        public:

            cOggDecoder(IDataSource* stream);
            ~cOggDecoder();

            //!Retruns the format of the audio data
            virtual AudioFormats getFormat();

            //!Returns the frequency of the audio data
            virtual int getFrequency();

            //!Returns whether seeking is supported
            virtual bool isSeekingSupported();

            //!Reads a section of data out of the audio stream
            virtual int readAudioData(void* output, int amount);

            //!Sets the position to read data out of
            virtual bool setPosition(int position, bool relative);

            //!If seeking is supported, will seek the stream to seconds
            virtual bool seek(int seconds,bool relative);

        protected:
       	    //!Callbacks used for read memory
            ov_callbacks     vorbisCallbacks;
	    //!some formatting data 
            vorbis_info*     vorbisInfo;
	    //!User Comments
            vorbis_comment*  vorbisComment;
	    //!Stream handle
            OggVorbis_File   oggStream; 
            bool seekable;
    };

}

#endif //! COGGDECODER_H_INCLUDED
