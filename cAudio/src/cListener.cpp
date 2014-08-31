// Copyright (c) 2008-2011 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones, Murat (wolfmanfx) Sari
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "cListener.h"
#include "cEFXFunctions.h"
#include "cOpenALUtil.h"

namespace cAudio
{
	void cListener::setPosition(const cVector3& pos)
	{
		cAudioMutexBasicLock lock(Mutex);
		Position = pos;
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
        checkALError();
	}
	void cListener::setDirection(const cVector3& dir)
	{
		cAudioMutexBasicLock lock(Mutex);
		Direction = dir;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
        checkALError();
	}
	void cListener::setUpVector(const cVector3& up)
	{
		cAudioMutexBasicLock lock(Mutex);
		UpVector = up;
		float orient[6] = {Direction[0], Direction[1], Direction[2], UpVector[0], UpVector[1], UpVector[2]};
		alListenerfv(AL_ORIENTATION, orient);
        checkALError();
	}
	void cListener::setVelocity(const cVector3& vel)
	{
		cAudioMutexBasicLock lock(Mutex);
		Velocity = vel;
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
        checkALError();
	}
	void cListener::setMasterVolume(const float& volume)
	{
		cAudioMutexBasicLock lock(Mutex);
		MasterGain = volume;
		alListenerf(AL_GAIN, MasterGain);
        checkALError();
	}
	void cListener::move(const cVector3& pos)
	{
		cAudioMutexBasicLock lock(Mutex);
		Velocity = pos - Position;
		Position = pos;
		
		alListener3f(AL_POSITION, Position.x, Position.y, Position.z);
		alListener3f(AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
        checkALError();
	}
#if CAUDIO_EFX_ENABLED == 1
	void cListener::setMetersPerUnit(const float& meters)
	{
		cAudioMutexBasicLock lock(Mutex);
		alListenerf(AL_METERS_PER_UNIT, meters);
        checkALError();
	}

	float cListener::getMetersPerUnit(void) const
	{
		float value = 1.0f;
		alGetListenerf(AL_METERS_PER_UNIT, &value);
		return value;
	}
#endif
};
