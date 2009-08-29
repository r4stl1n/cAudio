//****************************************************************
//cAudio 1.7.1 Tutorial 2
//Basic 3d Audio. Moving Audio source. Must be mono sound source
//****************************************************************

#include <iostream>
#include <math.h>
//Include IAudioManager so we can easily work with cAudio
#include "../../include/IAudioManager.h"
//Include IAudio so we can create cAudio objects
#include "../../include/IAudio.h"
//Include The cAudio vector class
#include "../../include/cVector3.h"
//Include our version of Sleep to free CPU usage
#include "../../include/cAudioSleep.h"

using namespace std;

int main(int argc, char* argv[])
{
    //To make visual studio happy
	cAudio::IAudio* mysound;
    //Some fancy text
    cout << "cAudio 1.7.1 Tutorial 2: 3dSound \n";
    //Hold audio source x position
    float rot = 0;
    //Grap the cAudioManager
    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    //Init the cAudio Engine
    manager->init(argc,argv);

	//Grab the listener object, which allows us to manipulate where "we" are in the world
	//It's useful to bind this to a camera if you are using a 3d graphics engine
	cAudio::IListener* listener = manager->getListener();

    //Create a IAudio object and load a sound from a file
    mysound = manager->createFromFile("bling","../../bin/bling.ogg",true);

    //Set the IAudio Sound to play3d and loop
    //play3d takes 4 arguments play3d(toloop,x,y,z,strength)
	if(mysound && listener)
	{
		listener->setPosition(cAudio::cVector3(0,0,0));
		mysound->play3d(cAudio::cVector3(0,0,0),1.0f,true);
		mysound->setVolume(2.0);

		while(mysound->playing())
		{
			//Figure out the location of our rotated sound
			rot+=0.1f * 0.017453293f;  //0.1 degrees a frame

			//Sound "starts" at x=5, y=0, z=0
			float x = 5.0f * cosf(rot) - 0.0f * sinf(rot);
			float z = 0.0f * cosf(rot) + 5.0f * sinf(rot);
			mysound->setPosition(cAudio::cVector3(x,0.0,z));

			//Sleep for 1 ms to free some CPU
			cAudio::cAudioSleep(1);
		}
	}

    //Delete all IAudio sounds
    manager->release();
    //Shutdown cAudio
    manager->shutDown();
    return 0;
}
