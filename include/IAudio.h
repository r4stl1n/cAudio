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

		//! Plays the source with the default or last set values
		virtual bool play() = 0;
		//! Plays the source in 2D mode
		virtual bool play2d(const bool& toLoop = false) = 0;
		//! Plays the source in 3D mode
		virtual bool play3d(const cVector3& position, const float& soundstr = 1.0 , const bool& toLoop = false) = 0;
		
		//! Pauses playback of the sound source 
		virtual void pause() = 0;
		//! Stops playback of the sound source       
		virtual void stop() = 0;
		//! Controls whether the source should loop or not
		virtual void loop(const bool& toLoop) = 0;
		//! Seeks through the audio stream to a specific spot
		/** Note: May not be supported by all codecs
		\param seconds: Number of seconds from the start of the audio stream to seek to
		\return True on success, False if the codec does not support seeking. */
		virtual bool seek(const float& seconds) = 0;

		//! Normally called every frame by the audio manager to update the internal buffers
		//! Note: For internal use only.
		virtual bool update() = 0;
		//! Releases all resources used by the audio source, normally used to clean up before deletion
		//! Note: For internal use only.
		virtual void release() = 0;

		//! Returns if the source is ready to be used
		virtual const bool isValid() const = 0;
		//! Returns if the source is playing
		virtual const bool isPlaying() const = 0;
		//! Returns if the source is paused
		virtual const bool isPaused() const = 0;
		//! Returns if the source is stopped
		virtual const bool isStopped() const = 0;
		//! Returns if the source is looping
		virtual const bool isLooping() const = 0;

		//! Sets the position of the source in 3D space       
		virtual void setPosition(const cVector3& position) = 0;
		//! Sets the current velocity of the source for doppler effects
		virtual void setVelocity(const cVector3& velocity) = 0;
		//! Sets the direction the source is facing
		virtual void setDirection(const cVector3& direction) = 0;

		//! Sets the factor used in attenuating the source over distance
		//! Larger values make it attenuate faster, smaller values make the source carry better
		//! Range: 0.0f to +inf (Default: 1.0f)
		virtual void setRolloffFactor(const float& rolloff) = 0;
		//! Sets how well the source carries over distance
		//! Same as setRolloffFactor(1.0f/soundstrength)
		//! Range: 0.0f to +inf (Default: 1.0f)
		virtual void setStrength(const float& soundstrength) = 0;
		//! Sets the distance from the source where attenuation will begin
		//! Range: 0.0f to +inf
		virtual void setMinDistance(const float& minDistance) = 0;
		//! Sets the distance from the source where attenuation will stop
		//! Range: 0.0f to +inf
		virtual void setMaxDistance(const float& maxDistance) = 0;

		//! Sets the pitch of the source
		virtual void setPitch(const float& pitch) = 0;
		//! Sets the source volume before attenuation and other effects
		//! Range: 0.0f to +inf (Default: 1.0f)
		virtual void setVolume(const float& volume) = 0;
		//! Sets the minimum volume that the source can be attenuated to
		//! Range: 0.0f to +inf (Default: 0.0f)
		virtual void setMinVolume(const float& minVolume) = 0;
		//! Sets the maximum volume that the source can achieve
		//! Range: 0.0f to +inf (Default: 1.0f)
		virtual void setMaxVolume(const float& maxVolume) = 0;

		//! Sets the angle of the inner sound cone of the source
		//! Note: This causes the sound to be loudest only if the listener is inside this cone
		//! Range: 0.0f to 360.0f (Default: 360.0f)
		virtual void setInnerConeAngle(const float& innerAngle) = 0;
		//! Sets the angle of the outer sound cone of the source
		//! Note: If the listener is outside of this cone, the sound cannot be heard.  Between the inner cone angle and this angle, the sound volume will fall off
		//! Range: 0.0f to 360.0f (Default: 360.0f)
		virtual void setOuterConeAngle(const float& outerAngle) = 0;
		//! Sets how much the volume of the source is scaled in the outer cone
		//! Range: 0.0f to +inf (Default: 0.0f)
		virtual void setOuterConeVolume(const float& outerVolume) = 0;

		//! Sets the doppler strength, which enhances or diminishes the doppler effect 
		//! Range: 0.0f to +inf (Default: 1.0f)
		virtual void setDopplerStrength(const float& dstrength) = 0; 
		//! Overrides the doppler velocity vector
		virtual void setDopplerVelocity(const cVector3& dvelocity) = 0;

		//!Returns the audio objects position
		virtual const cVector3 getPosition() const = 0;
		//!Returns the audio objects velocity
		virtual const cVector3 getVelocity() const = 0;
		//!Returns the audio objects direction
		virtual const cVector3 getDirection() const = 0;

		//! Returns the factor used in attenuating the source over distance
		virtual const float getRolloffFactor() const = 0;
		//! Returns the strength of the source
		virtual const float getStrength() const = 0;
		//! Returns the distance from the source where attenuation will begin
		virtual const float getMinDistance() const = 0;
		//! Returns the distance from the source where attenuation will stop
		virtual const float getMaxDistance() const = 0;

		//!Returns the pitch of the source
		virtual const float getPitch() const = 0;
		//!Returns the source volume before attenuation and other effects
		virtual const float getVolume() const = 0;
		//! Returns the minimum volume that the source can be attenuated to
		virtual const float getMinVolume() const = 0;
		//! Returns the maximum volume that the source can achieve
		virtual const float getMaxVolume() const = 0;

		//! Returns the angle of the inner sound cone of the source
		virtual const float getInnerConeAngle() const = 0;
		//! Returns the angle of the outer sound cone of the source
		virtual const float getOuterConeAngle() const = 0;
		//! Returns how much the volume of the source is scaled in the outer cone
		virtual const float getOuterConeVolume() const = 0;

		//!Returns the doppler strength, which enhances or diminishes the doppler effect 
		virtual const float getDopplerStrength() const = 0;
		//!Returns the override for the doppler velocity vector
		virtual const cVector3 getDopplerVelocity() const = 0;

    protected:
    private:
    };
};
#endif //! IAUDIO_H
