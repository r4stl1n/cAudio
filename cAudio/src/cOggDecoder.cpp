// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "../Headers/cOggDecoder.h"

#if CAUDIO_COMPILE_WITH_OGG_DECODER == 1

namespace cAudio
{
    //! Read Vorbis Data
    size_t VorbisRead(void *ptr, size_t byteSize,size_t sizeToRead, void *datasource)
    {
        IDataSource* Stream = (IDataSource*)datasource;
        return Stream->read(ptr, (int)(byteSize*sizeToRead));
    }

    //! Seek Vorbis Data
    int VorbisSeek(void *datasource,ogg_int64_t offset,int whence)
    {
        IDataSource* Stream = (IDataSource*)datasource;
        switch (whence)
        {
        case SEEK_SET:
            Stream->seek((int)offset, false);
            break;

        case SEEK_CUR:
            Stream->seek((int)offset, true);
            break;

        case SEEK_END:
            Stream->seek((int)(Stream->getSize()-offset), false);
            break;
        };
        return 0;
    }

    //! Returns the vorbis data that was stored.
    long VorbisTell(void *datasource)
    {
        return ((IDataSource*)datasource)->getCurrentPos();
    }
	
	int VorbisCloseFunc(void *datasource)
	{
		return 0;
	}

    cOggDecoder::cOggDecoder(IDataSource* stream) : IAudioDecoder(stream)
    {
        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = VorbisCloseFunc;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;
        Valid = (ov_open_callbacks(Stream,&oggStream,NULL,0,vorbisCallbacks) == 0);

		if(Valid)
		{
			vorbisInfo = ov_info(&oggStream, -1);
			vorbisComment = ov_comment(&oggStream,-1);
		}
    }

    cOggDecoder::~cOggDecoder()
    {
        ov_clear(&oggStream);
    }

    AudioFormats cOggDecoder::getFormat()
    {
		if(Valid)
		{
			if(vorbisInfo->channels == 1)
			{
				return EAF_16BIT_MONO;
			}
			else
			{
				return EAF_16BIT_STEREO;
			}
		}
		return EAF_8BIT_MONO;
    }

    int cOggDecoder::getFrequency()
    {
		if(Valid)
		{
			return vorbisInfo->rate;
		}
		return 0;
    }

    bool cOggDecoder::isSeekingSupported()
    {
		if(Valid)
		{
			return (ov_seekable(&oggStream)!=0);
		}
		return false;
    }

	bool cOggDecoder::isValid()
	{
		return Valid;
	}

    int cOggDecoder::readAudioData(void* output, int amount)
    {
		if(Valid)
		{
			int temp = 0;
			return ov_read(&oggStream,(char*)output,amount,0,2,1,&temp);
		}
		return 0;
    }

    bool cOggDecoder::setPosition(int position, bool relative)
    {
		if(Valid)
		{
			if(ov_seekable(&oggStream))
			{
				return (ov_raw_seek(&oggStream,position)==0);
			}
		}
		return false;
    }

    bool cOggDecoder::seek(float seconds, bool relative)
    {
		if(Valid)
		{
			if(ov_seekable(&oggStream))
			{
				if(relative)
				{
					double curtime = ov_time_tell(&oggStream);
					return (ov_time_seek(&oggStream,curtime+seconds)==0);
				}
				else
					return (ov_time_seek(&oggStream,seconds)==0);
			}
		}
        return false;
    }

	float cOggDecoder::getTotalTime()
	{
		return (float)ov_time_total(&oggStream, -1);
	}

	int cOggDecoder::getTotalSize()
	{
        // ov_pcm_total is in samples
		return (int)ov_pcm_total(&oggStream, -1) * vorbisInfo->channels * 2;
	}

	int cOggDecoder::getCompressedSize()
	{
		return (int)ov_raw_total(&oggStream, -1);
	}

	float cOggDecoder::getCurrentTime()
	{
		return (float)ov_time_tell(&oggStream);
	}

	int cOggDecoder::getCurrentPosition()
	{
        // ov_pcm_tell is in samples
		return (int)ov_pcm_tell(&oggStream) * vorbisInfo->channels * 2;
	}

	int cOggDecoder::getCurrentCompressedPosition()
	{
		return (int)ov_raw_tell(&oggStream);
	}
	
	cAudioString cOggDecoder::getType() const
	{
		return cAudioString(_CTEXT("cOggDecoder"));
	}
};

#endif
