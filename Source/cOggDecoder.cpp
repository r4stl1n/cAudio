#include "../Headers/cOggDecoder.h"
namespace cAudio
{
    //!Read Vorbis Data
    size_t VorbisRead(void *ptr, size_t byteSize,size_t sizeToRead, void *datasource)
    {
        IDataSource* Stream = (IDataSource*)datasource;
        return Stream->read(ptr,byteSize*sizeToRead);
    }

    //!Seek Vorbis Data
    int VorbisSeek(void *datasource,ogg_int64_t offset,int whence)
    {
        IDataSource* Stream = (IDataSource*)datasource;
        switch (whence)
        {
        case SEEK_SET:
            Stream->seek(offset, false);
            break;

        case SEEK_CUR:
            Stream->seek(offset, true);
            break;

        case SEEK_END:
            Stream->seek(Stream->getSize()-offset, false);
            break;
        };
        return 0;
    }

    //!Returns the vorbis data that was stored.
    long VorbisTell(void *datasource)
    {
        return ((IDataSource*)datasource)->getCurrentPos();
    }

    cOggDecoder::cOggDecoder(IDataSource* stream) : IAudioDecoder(stream)
    {
        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = NULL;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;
        ov_open_callbacks(Stream,&oggStream,NULL,0,vorbisCallbacks);

        vorbisInfo = ov_info(&oggStream, -1);
        vorbisComment = ov_comment(&oggStream,-1);
    }

    cOggDecoder::~cOggDecoder()
    {
        ov_clear(&oggStream);
    }
    //!Returns given vorbis channel format
    AudioFormats cOggDecoder::getFormat()
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
    //!Returns vorbis file frequency
    int cOggDecoder::getFrequency()
    {
        return vorbisInfo->rate;
    }

    //!Returns if vorbis file is seekable
    bool cOggDecoder::isSeekingSupported()
    {
        return (ov_seekable(&oggStream)!=0);
    }

    //!Reads the vorbis data
    int cOggDecoder::readAudioData(void* output, int amount)
    {
        int temp = 0;
		int result = ov_read(&oggStream,(char*)output,amount,0,2,1,&temp);
		return (result < 0) ? 0 : result;
    }

    //!Sets the postion for vorbis data reader
    bool cOggDecoder::setPosition(int position, bool relative)
    {
        if(ov_seekable(&oggStream))
        {
            return (ov_raw_seek(&oggStream,position)==0);
        }
        else
            return false;
    }

    //!Seeks the vorbis data
    bool cOggDecoder::seek(float seconds, bool relative)
    {
        if(ov_seekable(&oggStream))
        {
			if(relative)
			{
				float curtime = ov_time_tell(&oggStream);
				return (ov_time_seek(&oggStream,curtime+seconds)==0);
			}
			else
				return (ov_time_seek(&oggStream,seconds)==0);
        }
        return false;
    }
}
