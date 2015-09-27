
#include "cAudioSource.h"

namespace cAudio
{
    class cAudioBuffer : public IAudioBuffer
    {
        ALuint Buffer;
        bool   Valid;
        float  TotalTime;

    public:

        cAudioBuffer(IAudioDecoder *decoder)
        {
            Buffer    = 0;
            Valid     = false;
            TotalTime = decoder->getTotalTime();
            
            decoder->setPosition(0, false);
            
            const int totalSize = decoder->getTotalSize()&~1;
            if (totalSize > 0)
            {
                cAudioVector<char>::Type data(totalSize, 0);

                int      totalread  = 0;
                unsigned errorcount = 0;
                while (totalread < totalSize)
                {
                    const int actualread = decoder->readAudioData(&data[totalread], totalSize - totalread);
                    if (actualread > 0) {
                        totalread += actualread;
                    } else if (actualread < 0) {
                        ++errorcount;
                        getLogger()->logDebug("Audio Buffer", "Decoder returned an error: %i (%i of 3)", actualread, errorcount);
                        if(errorcount >= 3) {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                if (totalread != totalSize) {
                    getLogger()->logError("Audio Buffer", "Stopped short");
                }

                alGenBuffers(1, &Buffer);
                checkALError();
                const ALsizei freq   = decoder->getFrequency();
                const ALenum  format = convertAudioFormatEnum(decoder->getFormat());
                getLogger()->logDebug("Audio Buffer", "Buffered %d bytes of data into buffer %d at %d hz.", totalread, (int)Buffer, (int)freq);
                alBufferData(Buffer, format, &data[0], data.size(), freq);
                checkALError();
                Valid = true;
            }
        }

        ~cAudioBuffer()
        {
            if (Valid)
            {
                alDeleteBuffers(1, &Buffer);
            }
        }

        bool     isValid()  const { return Valid; }
        unsigned getBuffer() const { return Buffer; }

        int getChannels() const
        {
            ALint channels = 0;
            alGetBufferi(Buffer, AL_CHANNELS, &channels);
            return channels;
        }

        int getTotalAudioSize() const
        {
            ALint size = -1;
            alGetBufferi(Buffer, AL_SIZE, &size);
            return size;
        }

        float getTotalAudioTime() const { return TotalTime; }
    };


    class cAudioStaticSource : public cAudioSourceBase
    {
        IAudioBuffer* Buffer;
        
    public:
		cAudioStaticSource(IAudioBuffer* buffer, IAudioDeviceContext* context)
            : cAudioSourceBase(context), Buffer(NULL)
        {
            setBuffer(buffer);
        }

        ~cAudioStaticSource()
        {
            if (Buffer)
                Buffer->drop();
        }

        bool setBuffer(IAudioBuffer* buffer)
        {
            if (isPlaying())
                stop();
            
            if (Buffer)
                Buffer->drop();

            Buffer = buffer;

            if (Buffer) {
                Buffer->grab();
                alSourcei(Source, AL_BUFFER, Buffer->getBuffer());
                checkALError();
            }
            return true;
        }

        IAudioBuffer *getBuffer()
        {
            return Buffer;
        }

        bool play()
        {
            alSourcePlay(Source);
            checkALError();
            getLogger()->logDebug("Audio Static Source", "Source playing.");
            signalEvent(ON_PLAY);
            oldState = AL_PLAYING;
            return true;
        }

        bool play2d(const bool& toLoop)
        {
            alSourcei(Source, AL_SOURCE_RELATIVE, true);
            alSourcei(Source, AL_LOOPING, toLoop);
            checkALError();
            return play();
        }

        bool play3d(const cVector3& position, const float& soundstr, const bool& toLoop)
        {
            alSourcei(Source, AL_SOURCE_RELATIVE, false);
            alSourcei(Source, AL_LOOPING, toLoop);
            checkALError();
            setPosition(position);
            setStrength(soundstr);
            return play();
        }
		
        void pause()
        {
            alSourcePause(Source);
            checkALError();
            getLogger()->logDebug("Audio Static Source", "Source paused.");
            signalEvent(ON_PAUSE);
            oldState = AL_PAUSED;
        }

        void stop()
        {
            alSourceStop(Source);
            alSourceRewind(Source);
            checkALError();
            getLogger()->logDebug("Audio Static Source", "Source stopped.");
            signalEvent(ON_STOP);
            oldState = AL_STOPPED;
        }

        void loop(const bool& toLoop)
        {
            alSourcei(Source, AL_LOOPING, toLoop);
            checkALError();
        }

		bool seek(const float& seconds, bool relative)
        {
            float start = relative ? getCurrentAudioTime() : 0.f;
            alSourcef(Source, AL_SEC_OFFSET, start + seconds);
            checkALError();
            return true;
        }

        float getTotalAudioTime() { return Buffer ? Buffer->getTotalAudioTime() : 0.f; }
        int   getTotalAudioSize() { return Buffer ? Buffer->getTotalAudioSize() : 0; }

        int getCompressedAudioSize() { return -1; }

		float getCurrentAudioTime()
        {
            float time = -1;
            alGetSourcef(Source, AL_SEC_OFFSET, &time);
            return time;
        }

        int getCurrentAudioPosition()
        {
            int offset = -1;
            alGetSourcei(Source, AL_BYTE_OFFSET, &offset);
            return offset;
        }

		int getCurrentCompressedAudioPosition() { return -1; }

        bool update()
        {
            if(isValid() || isPlaying()) {
                signalEvent(ON_UPDATE);
            }

            ALenum state;
            alGetSourcei(Source, AL_SOURCE_STATE, &state);
            checkALError();
            if(state == AL_STOPPED && oldState != state)
            {
                getLogger()->logDebug("Audio Static Source", "Source stopped.");
                signalEvent(ON_STOP);
                oldState = state;
            }
            return state != AL_STOPPED;
        }

        bool isValid() const
        {
            return Buffer ? Buffer->isValid() : false;
        }

        bool isLooping() const
        {
            ALint looping = false;
            alGetSourcei(Source, AL_LOOPING, &looping);
            return looping == AL_TRUE;
        }

#if CAUDIO_EFX_ENABLED == 1
		virtual unsigned int getNumEffectSlotsAvailable() const { return 0; }
		virtual bool attachEffect(unsigned int slot, IEffect* effect) { return false; }
		virtual void removeEffect(unsigned int slot) {}
		virtual bool attachFilter(IFilter* filter) { return false; }
		virtual void removeFilter() {}
#endif
    };
}
