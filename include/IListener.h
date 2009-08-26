
#ifndef ILISTENER_H
#define ILISTENER_H

#include "cVector3.h"

namespace cAudio
{
    class IListener
    {
    public:
		IListener() {}
        virtual ~IListener() {}

		//!Sets the position of the listener
		//!Note that this will automatically set velocity to 0
		//!Use move() if you'd like to have cAudio automatically handle velocity for you
		//!or remember to set it yourself after setPosition
		virtual void setPosition(const cVector3 pos) = 0;
		//!Sets the direction the listener is facing
		virtual void setDirection(const cVector3 dir) = 0;
		//!Sets the up vector to use for the listener
		virtual void setUpVector(const cVector3 up) = 0;
		//!Sets the current velocity of the listener for doppler effects
		virtual void setVelocity(const cVector3 vel) = 0;
		//!Sets the global volume modifier (will effect all sources)
		virtual void setMasterVolume(const float volume) = 0;

		//!Convenience function to automatically set the velocity for you on a move
		//!Velocity will be set to new position - last position
		virtual void move(const cVector3 pos) = 0;

		//!Returns the current position of the listener
		virtual cVector3 getPosition(void) const = 0;
		//!Returns the current direction of the listener
		virtual cVector3 getDirection(void) const = 0;
		//!Returns the current up vector of the listener
		virtual cVector3 getUpVector(void) const = 0;
		//!Returns the current velocity of the listener
		virtual cVector3 getVelocity(void) const = 0;
		//!Returns the global volume modifier for all sources
		virtual float getMasterVolume(void) const = 0;

    protected:
    private:
    };
}
#endif //! ILISTENER_H
