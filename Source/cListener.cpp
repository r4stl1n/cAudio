#include "../Headers/cListener.h"
#include <AL/al.h>

namespace cAudio
{
	void cListener::setPosition(const cVector3 pos)
	{
		Position = pos;
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
	}
	void cListener::setDirection(const cVector3 dir)
	{
		Direction = dir;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
	}
	void cListener::setUpVector(const cVector3 up)
	{
		UpVector = up;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
	}
	void cListener::setVelocity(const cVector3 vel)
	{
		Velocity = vel;
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
	}
	void cListener::setMasterVolume(const float volume)
	{
		MasterGain = volume;
		alListenerf(AL_GAIN, MasterGain);
	}
	void cListener::move(const cVector3 pos)
	{
		Velocity = pos - Position;
		Position = pos;
		
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
	}
};