#include <iostream>
#include <stdio.h>
#include <string.h>
#include "../Headers/cAudio.h"

namespace cAudio
{
    cAudio::cAudio(IAudioDecoder* decoder) : Decoder(decoder)
    {
        streaming = false;
        playaudio = false;
        pauseaudio = false;
		toloop = false;

		//Generates 3 buffers for the ogg file
		alGenBuffers(3, buffers);
		//Creates one source to be stored.
		alGenSources(1, &source);
    }

    cAudio::~cAudio()
    {
        delete Decoder;
    }

    //!Stops all playing sound sources and deletes the sources and buffers
    void cAudio::release()
    {
		//Stops the audio source
		alSourceStop(source);
		empty();
		//Deletes the source
		alDeleteSources(1, &source);
		//deletes the last filled buffer
		alDeleteBuffers(3, buffers);
    }

    //!Plays back sound source
    bool cAudio::playback()
    {
        return playing();
    }

    bool cAudio::paused()
    {
        ALenum state = 0;

        alGetSourcei(source, AL_SOURCE_STATE, &state);

        return (state == AL_PAUSED);
    }

    //!checks to see if audio source is playing if it is returns true
    bool cAudio::playing()
    {
        ALenum state = 0;

        alGetSourcei(source, AL_SOURCE_STATE, &state);

        return (state == AL_PLAYING);
    }

    //!updates the sound source by refilling the buffers with ogg data.
    bool cAudio::update()
    {
        if(!isvalid() || !playing())
            return false;

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
        return (Decoder != 0);
    }

    //!Sets the sound source relativity to follow the listener to give the illusion of stereo 2d sound
    void cAudio::play2d(bool loop)
    {
        alSourcei (source, AL_SOURCE_RELATIVE, true);
        toloop = loop;
        play();
		alSourcePlay(source);
    }

    //!Plays the given audio file with 3d position
    void cAudio::play3d(cVector3 position, float soundstr, bool loop)
    {
		this->position = position;
		this->strength = soundstr;
        alSourcei (source, AL_SOURCE_RELATIVE, false);
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        alSourcef (source, AL_ROLLOFF_FACTOR,  soundstr);
        toloop = loop;
        play();
        alSourcePlay(source);
    }

    //!Used to tell the soundsource to loop or not
    void cAudio::loop(bool loop)
    {
        toloop = loop;
    }

    //!Used to move the audio sources position after the initial creation
    void cAudio::setPosition(const cVector3 position)
    {
		this->position = position;
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
    }

    //!Used to set the velocity of the audio source.
    void cAudio::setVelocity(const cVector3 velocity)
    {
		this->velocity = velocity;
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    //!Used to set the direction of the audio source
    void cAudio::setDirection(const cVector3 direction)
    {
		this->direction = direction;
        alSource3f(source, AL_DIRECTION, direction.x, direction.y, direction.z);
    }

    //!Used to set the sound strength or roll off factor
    void cAudio::setStrength(const float soundstrength)
    {
        alSourcef(source, AL_ROLLOFF_FACTOR, soundstrength);
    }

    //!Used to set the pitch of the audio file
    void cAudio::setPitch(const float pitch)
    {
		this->pitch = pitch;
        alSourcef (source, AL_PITCH, pitch);
    }

    //!Used to set the volume of the audio source
    void cAudio::setVolume(const float volume)
    {
		this->volume = volume;
        alSourcef(source, AL_GAIN, volume);
    }

    //!Used to set the doppler strength of the audio sources doppler effect
    void cAudio::setDopplerStrength(const float dstrength)
    {
		this->dstrength = dstrength;
        alSourcef(source, AL_DOPPLER_FACTOR, dstrength);
    }

    //!Used to set the doppler velocity of the audio source
    void cAudio::setDopplerVelocity(const cVector3 dvelocity)
    {
		this->dvelocity = dvelocity;
        alSource3f(source, AL_DOPPLER_VELOCITY, dvelocity.x, dvelocity.y, dvelocity.z);
    }

	cVector3 cAudio::getPosition()
	{
		return this->position;
	}
	
	cVector3 cAudio::getVelocity(){
		return this->velocity;
	}
		
	cVector3 cAudio::getDirection(){
		return this->direction;
	}
	
	float cAudio::getDopplerStrength(){
		return this->dstrength;
	}
				
	float cAudio::getStrength(){
		return this->strength;
	}
		
	float cAudio::getVolume(){
		return this->volume;
	}
	
	float cAudio::getPitch(){
		return this->pitch;
	}
		
	bool cAudio::isLooping(){
		return this->toloop;
	}

    //!Allows us to seek through a stream
    void cAudio::seek(float secs)
    {
        if(Decoder->isSeekingSupported())
        {
            Decoder->seek(secs, false);
        }
    }

    //!Used to play the audio source
    bool cAudio::play()
    {
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
        return true; 
    }

    //!Used to stop the audio source
    void cAudio::stop()
    {
        playaudio = false;
        alSourceStop(source);
    }

    //!Used to pause the audio source
    void cAudio::pause()
    {
        playaudio = false;
        alSourcePause(source);
    }

}
