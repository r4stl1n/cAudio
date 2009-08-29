#include "../Headers/cListener.h"
#include <AL/al.h>

namespace cAudio
{
	void cListener::setPosition(const cVector3 pos)
	{
		Mutex.lock();
		Position = pos;
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
		Mutex.unlock();
	}
	void cListener::setDirection(const cVector3 dir)
	{
		Mutex.lock();
		Direction = dir;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
		Mutex.unlock();
	}
	void cListener::setUpVector(const cVector3 up)
	{
		Mutex.lock();
		UpVector = up;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
		Mutex.unlock();
	}
	void cListener::setVelocity(const cVector3 vel)
	{
		Mutex.lock();
		Velocity = vel;
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
		Mutex.unlock();
	}
	void cListener::setMasterVolume(const float volume)
	{
		Mutex.lock();
		MasterGain = volume;
		alListenerf(AL_GAIN, MasterGain);
		Mutex.unlock();
	}
	void cListener::move(const cVector3 pos)
	{
		Mutex.lock();
		Velocity = pos - Position;
		Position = pos;
		
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
		Mutex.unlock();
	}
};