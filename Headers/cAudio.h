#ifndef CAUDIO_H_INCLUDED
#define CAUDIO_H_INCLUDED
#include <string>
#include <iostream>
#include <vector>
#include "AL/al.h"
#include "AL/alut.h"

#define BUFFER_SIZE ( 1024 * 32 )
#include "../include/IAudio.h"
#include "../Include/cVector3.h"

namespace cAudio
{
    class cAudio : public IAudio
    {
    public:
		//! play with defualts / the last set values
		bool play(); 
		//!plays the audio file 2d no distance.
		void play2d(bool loop = false);
		//!plays the audio file and sets it to 3d
		void play3d(cVector3 position, float soundstr = 1.0 , bool loop = false);

		//!allows us to set the position or reset the position
		void setPosition(cVector3 position);
		//!allows you to set the audio objects velocity
		void setVelocity(cVector3 velocity);
		//!allows us to set the direction the audio should play in
		void setDirection(cVector3 direction);
		//! Sets the audios pitch level
		void setPitch(float pitch); 
		//!allows us to set and reset the sound strenght
		void setStrength(float soundstrength);
		//!Set the volume
		void setVolume(float volume);
		//!Set the doppler strength
		void setDopplerStrength(float doop);
		//!Set doppler velocity
		void setDopplerVelocity(cVector3 dvelocity);

		//!Seek the audio stream
		void seek(float secs);
		//!release the file handle
		void release();
		//!pauses the audio file
		void pause();
		//!controls altering of the looping to make it loop or not to.
		void loop(bool loop);
		//!stops the audio file from playing
		void stop();

		//!returns if playing
		bool playback();
		//!check if source is playing
		bool playing();
		//!update the stream
		bool update();
		//!checks to make sure everything is ready to go
		bool isvalid();

		cAudio(IAudioDecoder* decoder);
		~cAudio();

	protected:
	private:
		//!empties the queue
		void empty();
		//!checks openal error state
		void check();
		//!reloads a buffer
		bool stream(ALuint buffer);
		//!stringify an error code
		std::string errorString(int code);

		//! front and back buffers
		ALuint buffers[3]; 
		//! audio source        
		ALuint source; 
		//! decoder pointer        
		IAudioDecoder* Decoder;

		//! if the file to be played is going to be streamed
		bool streaming;
		//!if the file is to loop 
		bool toloop;
		//!if the file should play
		bool playaudio;
		//!if to pause audio
		bool pauseaudio;
		//!if audio is paused
		bool paused();
		//! Stores the position of the audio object
		cVector3 position;
		//! Stores the velocity of the audio object
		cVector3 velocity;
		//! Stores the direction of the audio object
		cVector3 direction;
		//! Stores the doppler velocity
		cVector3 dvelocity;
    };
}
#endif //! CAUDIO_H_INCLUDED
