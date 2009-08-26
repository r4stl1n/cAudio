#ifndef CLISTENER_H_INCLUDED
#define CLISTENER_H_INCLUDED

#include "../include/IListener.h"

namespace cAudio
{
	class cListener : public IListener
    {
    public:
		cListener() :	Direction(cVector3(0.f, 0.f, -1.f)),
						UpVector(cVector3(0.f, 1.f, 0.f)),
						MasterGain(1.f) {}
		virtual ~cListener() {}

		//!Sets the position of the listener
		//!Note that this will automatically set velocity to 0
		//!Use move() if you'd like to have cAudio automatically handle velocity for you
		//!or remember to set it yourself after setPosition
		virtual void setPosition(const cVector3 pos);
		//!Sets the direction the listener is facing
		virtual void setDirection(const cVector3 dir);
		//!Sets the up vector to use for the listener
		virtual void setUpVector(const cVector3 up);
		//!Sets the current velocity of the listener for doppler effects
		virtual void setVelocity(const cVector3 vel);
		//!Sets the global volume modifier (will effect all sources)
		virtual void setMasterVolume(const float volume);
		//!Convenience function to automatically set the velocity for you on a move
		//!Velocity will be set to new position - last position
		virtual void move(const cVector3 pos);

		//!Returns the current position of the listener
		virtual cVector3 getPosition(void) const { return Position; }
		//!Returns the current direction of the listener
		virtual cVector3 getDirection(void) const { return Direction; }
		//!Returns the current up vector of the listener
		virtual cVector3 getUpVector(void) const { return UpVector; }
		//!Returns the current velocity of the listener
		virtual cVector3 getVelocity(void) const { return Velocity; }
		//!Returns the global volume modifier for all sources
		virtual float getMasterVolume(void) const { return MasterGain; }
	protected:
		cVector3 Position;
		cVector3 Direction;
		cVector3 UpVector;
		cVector3 Velocity;
		float MasterGain;
	private:
    };
}
#endif //! CLISTENER_H_INCLUDED
