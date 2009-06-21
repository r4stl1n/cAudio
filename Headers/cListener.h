#ifndef CLISTENER_H_INCLUDED
#define CLISTENER_H_INCLUDED
#include <AL/al.h>
#include <AL/alut.h>
//!#include <AL/alext.h>
//!#include <AL/efx.h>
#include <iostream>
#define LOAD_AL_FUNC(x) (x = (typeof(x))alGetProcAddress(#x))

namespace cAudio
{
    class cListener
    {
    public:
        cListener static  &getInstance()
        {
            static cListener This;
            return This;
        }

        //!Sets the listeners position
        void setPosition(ALfloat x,ALfloat y, ALfloat z)
        {
            alListener3f(AL_POSITION,x,y,z);
        }
	//!Sets the listeners orientation
        void setOrientation(ALfloat x,ALfloat y,ALfloat z,ALfloat upx,ALfloat upy,ALfloat upz)
        {
            ALfloat orientation[6] = {x,y,z,upx,upy,upz};
            alListenerfv(AL_ORIENTATION,orientation);
        }
	//!Inits Openal
        static void initaudio(int argc,char* argv[])
        {
            alutInit(&argc,argv);
	  /*
            ALCcontext *Context;
            ALCdevice *Device;
            ALint attribs[4] = {0};
            ALCint iSends = 0;
            LPALGENEFFECTS alGenEffects;
            LPALDELETEEFFECTS alDeleteEffects;
            LPALISEFFECT alIsEffect;

            Device = alcOpenDevice(NULL);

            //!Query for effect extention
            if(alcIsExtensionPresent(Device,"ALC_EXT_EFX") == AL_FALSE){

            return;
            }

            //!Use context creation hint to request 4 axiliary sends per source
            attribs[0] = ALC_MAX_AUXILIARY_SENDS;
            attribs[1] = 4;
            attribs[2] = 0;

            Context = alcCreateContext(Device,attribs);

            if(!Context)
                return;

            //!activate context

            alcMakeContextCurrent(Context);

            //!retrive Actual number aux sends avaliable per source
            alcGetIntegerv(Device,ALC_MAX_AUXILIARY_SENDS,1,&iSends);

            //!Get the effect extention function pointers
            LOAD_AL_FUNC(alGenEffects);
            LOAD_AL_FUNC(alDeleteEffects);
            LOAD_AL_FUNC(alIsEffect);

	*/
        }
	//!Shutsdown openal
        static void shutdownaudio()
        {
            alutExit();
        }
    };
}
#endif //! CLISTENER_H_INCLUDED
