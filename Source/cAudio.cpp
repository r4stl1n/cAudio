#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../Headers/cAudio.h"

namespace cAudio
{
    cAudio::cAudio(IAudioDecoder* decoder) : Decoder(decoder)
    {
		Mutex.lock();
        streaming = false;
        playaudio = false;
        pauseaudio = false;
		toloop = false;

		//Generates 3 buffers for the ogg file
		alGenBuffers(3, buffers);
		//Creates one source to be stored.
		alGenSources(1, &source);
		Mutex.unlock();
    }

    cAudio::~cAudio()
    {
		Mutex.lock();
        delete Decoder;
		Mutex.unlock();
    }

    //!Stops all playing sound sources and deletes the sources and buffers
    void cAudio::release()
    {
		Mutex.lock();
		//Stops the audio source
		alSourceStop(source);
		empty();
		//Deletes the source
		alDeleteSources(1, &source);
		//deletes the last filled buffer
		alDeleteBuffers(3, buffers);
		Mutex.unlock();
    }

    //!Plays back sound source
    bool cAudio::playback()
    {
		Mutex.lock();
		bool play = playing();
		Mutex.unlock();
        return play;
    }

    bool cAudio::paused()
    {
		Mutex.lock();
        ALenum state = 0;

        alGetSourcei(source, AL_SOURCE_STATE, &state);

		Mutex.unlock();
        return (state == AL_PAUSED);
    }

    //!checks to see if audio source is playing if it is returns true
    bool cAudio::playing()
    {
		Mutex.lock();
        ALenum state = 0;

        alGetSourcei(source, AL_SOURCE_STATE, &state);

		Mutex.unlock();
        return (state == AL_PLAYING);
    }

    //!updates the sound source by refilling the buffers with ogg data.
    bool cAudio::update()
    {
		Mutex.lock();
        if(!isvalid() || !playing())
		{
			Mutex.unlock();
            return false;
		}

        int processed = 0;
        bool active = true;
		//gets the sound source processed buffers/
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
		//while there is more data refill buffers with audio data.
		while (processed--)
        {
            ALuint buffer;

            alSourceUnqueueBuffers(source, 1, &buffer);

            active = stream(buffer);

			//if more in stream continue playing.
            if(active)
                alSourceQueueBuffers(source, 1, &buffer);
        }
		Mutex.unlock();
		return active;
    }

    //!The streaming function
    bool cAudio::stream(ALuint buffer)
    {
        if(Decoder)
        {
	        //stores the caculated data into buffer that is passed to output.
			size_t totalread = 0;
	        char tempbuffer[BUFFER_SIZE];
			while( totalread < BUFFER_SIZE )
			{
				char tempbuffer2[BUFFER_SIZE];
				int actualread = Decoder->readAudioData(tempbuffer2, BUFFER_SIZE-totalread);
				if(actualread > 0)
				{
					memcpy(tempbuffer+totalread,tempbuffer2,actualread);
					totalread += actualread;
				}
				if(actualread < 0)
				{
					break;
				}
				if(actualread == 0)
				{
					if(toloop)
					{
						//If we are to loop, set to the beginning and reload from the start
						Decoder->setPosition(0,false);
					}
					else
						break;
				}
			}

	        //Second check, in case looping is not enabled, we will return false for end of stream
	        if(totalread == 0)
	       	{
	       		 return false;
	        }
			//std::cout << buffer << std::endl;
            alBufferData(buffer, Decoder->getFormat(), tempbuffer, totalread, Decoder->getFrequency());
            return true;
        }
		return false;
    }

    //!clears the sound sources buffers and makes them free to be used by other sound sources.
    void cAudio::empty()
    {
        int queued = 0;
		//grabs allt he sources buffers.
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

        while (queued--)
        {
            ALuint buffer;
			//unqueues sources buffers to be used for others.
            alSourceUnqueueBuffers(source, 1, &buffer);
        }
    }

    //!Checks the given functions
    void cAudio::check()
    {
        int error = alGetError();

        if (error != AL_NO_ERROR)
        {
            std::cout<< "OpenAL error was Raised.";
        }
    }

    //!checks to see if the given ogg file is valid
    bool cAudio::isvalid()
    {
		Mutex.lock();
		bool state = (Decoder != 0);
		Mutex.unlock();
        return state;
    }

    //!Sets the sound source relativity to follow the listener to give the illusion of stereo 2d sound
    void cAudio::play2d(bool loop)
    {
		Mutex.lock();
        alSourcei (source, AL_SOURCE_RELATIVE, true);
        toloop = loop;
        play();
		alSourcePlay(source);
		Mutex.unlock();
    }

    //!Plays the given audio file with 3d position
    void cAudio::play3d(cVector3 position, float soundstr, bool loop)
    {
		Mutex.lock();
		this->position = position;
		this->strength = soundstr;
        alSourcei (source, AL_SOURCE_RELATIVE, false);
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        alSourcef (source, AL_ROLLOFF_FACTOR,  soundstr);
        toloop = loop;
        play();
        alSourcePlay(source);
		Mutex.unlock();
    }

    //!Used to tell the soundsource to loop or not
    void cAudio::loop(bool loop)
    {
		Mutex.lock();
        toloop = loop;
		Mutex.unlock();
    }

    //!Used to move the audio sources position after the initial creation
    void cAudio::setPosition(const cVector3 position)
    {
		Mutex.lock();
		this->position = position;
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
		Mutex.unlock();
    }

    //!Used to set the velocity of the audio source.
    void cAudio::setVelocity(const cVector3 velocity)
    {
		Mutex.lock();
		this->velocity = velocity;
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		Mutex.unlock();
    }

    //!Used to set the direction of the audio source
    void cAudio::setDirection(const cVector3 direction)
    {
		Mutex.lock();
		this->direction = direction;
        alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);
		Mutex.unlock();
    }

    //!Used to set the sound strength or roll off factor
    void cAudio::setStrength(const float soundstrength)
    {
		Mutex.lock();
        alSourcef(source, AL_ROLLOFF_FACTOR, soundstrength);
		Mutex.unlock();
    }

    //!Used to set the pitch of the audio file
    void cAudio::setPitch(const float pitch)
    {
		Mutex.lock();
		this->pitch = pitch;
        alSourcef (source, AL_PITCH, pitch);
		Mutex.unlock();
    }

    //!Used to set the volume of the audio source
    void cAudio::setVolume(const float volume)
    {
		Mutex.lock();
		this->volume = volume;
        alSourcef(source, AL_GAIN, volume);
		Mutex.unlock();
    }

    //!Used to set the doppler strength of the audio sources doppler effect
    void cAudio::setDopplerStrength(const float dstrength)
    {
		Mutex.lock();
		this->dstrength = dstrength;
        alSourcef(source, AL_DOPPLER_FACTOR, dstrength);
		Mutex.unlock();
    }

    //!Used to set the doppler velocity of the audio source
    void cAudio::setDopplerVelocity(const cVector3 dvelocity)
    {
		Mutex.lock();
		this->dvelocity = dvelocity;
        alSource3f(source, AL_DOPPLER_VELOCITY, dvelocity.x, dvelocity.y, dvelocity.z);
		Mutex.unlock();
    }

	const cVector3& cAudio::getPosition()const
	{
		return this->position;
	}
	
	const cVector3& cAudio::getVelocity()const
	{
		return this->velocity;
	}
		
	const cVector3& cAudio::getDirection()const
	{
		return this->direction;
	}
	
	const float& cAudio::getDopplerStrength()const
	{
		return this->dstrength;
	}
				
	const float& cAudio::getStrength()const
	{
		return this->strength;
	}
		
	const float& cAudio::getVolume()const
	{
		return this->volume;
	}
	
	const float& cAudio::getPitch()const
	{
		return this->pitch;
	}
		
	const bool& cAudio::isLooping()const
	{
		return this->toloop;
	}

    //!Allows us to seek through a stream
    void cAudio::seek(float secs)
    {
		Mutex.lock();
        if(Decoder->isSeekingSupported())
        {
            Decoder->seek(secs, false);
        }
		Mutex.unlock();
    }

    //!Used to play the audio source
    bool cAudio::play()
    {
		Mutex.lock();
        playaudio = true;
		if (!paused()) 
        { 
            int queueSize = 0; 
            for(int u = 0; u < 3; u++) 
            { 
                int val = stream(buffers[u]); 
 
                if(val < 0) 
                {  
                    return false; 
                } 
                else if(val > 0) 
                    ++queueSize; 
            } 
            //Stores the sources 3 buffers to be used in the queue 
            alSourceQueueBuffers(source, queueSize, buffers); 
        }
        alSourcePlay(source);
		Mutex.unlock();
        return true; 
    }

    //!Used to stop the audio source
    void cAudio::stop()
    {
		Mutex.lock();
        playaudio = false;
        alSourceStop(source);
		Mutex.unlock();
    }

    //!Used to pause the audio source
    void cAudio::pause()
    {
		Mutex.lock();
        playaudio = false;
        alSourcePause(source);
		Mutex.unlock();
    }

	const ALuint& cAudio::getSource()const
	{
		return this->source;
	}
}
