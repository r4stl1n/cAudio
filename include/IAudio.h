#ifndef IAUDIO_H
#define IAUDIO_H

#include "IAudioDecoder.h"
#include "cVector3.h"

namespace cAudio
{
    class IAudio
    {
    public:
		IAudio() {}
		virtual ~IAudio() {}

		//! play with defualts / the last set values
		virtual bool play() = 0; 
		//!plays the audio file 2d no distance.
		virtual void play2d(bool loop = false) = 0;
		//!plays the audio file and sets it to 3d
		virtual void play3d(cVector3 position, float soundstr = 1.0 , bool loop = false) = 0;
		//!allows us to set the position or reset the position        
		virtual void setPosition(const cVector3 position) = 0;
		//!allows you to set the audio objects velocity
		virtual void setVelocity(const cVector3 velocity) = 0;
		//!allows us to set the direction the audio should play in
		virtual void setDirection(const cVector3 direction) = 0;
		//! Sets the audios pitch level
		virtual void setPitch(const float pitch) = 0; 
		//!allows us to set and reset the sound strength
		virtual void setStrength(const float soundstrength) = 0;
		//! Set the volume
		virtual void setVolume(const float volume) = 0; 
		//!Set the doppler strength
		virtual void setDopplerStrength(const float dstrength) = 0; 
		//!Set the doppler velocity
		virtual void setDopplerVelocity(const cVector3 dvelocity) = 0;
		//!Seek through the audio stream        
		virtual void seek(float secs) = 0; 

		//!Returns the audio objects position
		virtual const cVector3& getPosition()const = 0;
		//!Returns the audio objects velocity
		virtual const cVector3& getVelocity()const = 0;
		//!Returns the audio objects direction
		virtual const cVector3& getDirection()const = 0;
		//!Returns the audio objects doppler strength
		virtual const float& getDopplerStrength()const = 0;
		//!Returns the audio objects strength
		virtual const float& getStrength()const = 0;
		//!Returns the volume of the sound object
		virtual const float& getVolume()const = 0;
		//!Returns the pitch volume
		virtual const float& getPitch()const = 0;
		//!Returns if the sound object is looping
		virtual const bool& isLooping()const = 0;

		//!release the file handle
		virtual void release() = 0;
		//!pauses the audio file
		virtual void pause() = 0;
		//!controls altering of the looping to make it loop or not to.
		virtual void loop(bool loop) = 0;
		//!stops the audio file from playing        
		virtual void stop() = 0;

		//!play file
		virtual bool playback() = 0;
		//!check if source is playing
		virtual bool playing() = 0;
		//!update the stream
		virtual bool update() = 0;
		//!checks to make sure everything is ready to go
		virtual bool isvalid() = 0;
    protected:
    private:
    };
}
#endif //! IAUDIO_H
