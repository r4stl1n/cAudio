#include <iostream>
//Include IAudioManager so we can easily work with cAudio
#include "../../include/IAudioManager.h"
//Include IAudio so we can create cAudio objects
#include "../../include/IAudio.h"
//Include our version of Sleep to free CPU usage
#include "../../include/cAudioSleep.h"

using namespace std;

int main(int argc, char* argv[])
{
    //To make visual studio happy
    cAudio::IAudio* mysound;

    //Some fancy text
    cout << "cAudio 1.7.1 Tutorial 1: 2DSound \n";

    //Grap the cAudioManager
    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    //Init the cAudio Engine
    manager->init(argc,argv);
    //Create a IAudio object and load a sound from a file
    mysound = manager->createFromFile("bling","../../bin/bling.ogg",true);

	if(mysound)
	{
		mysound->setVolume(0.5);
		//Set the IAudio Sound to play2d and loop
		mysound->play2d(true);

		//Sleep for 10,000 ms to let the sound play in the worker thread
		cAudio::cAudioSleep(10000);
	}

    //Delete all IAudio sounds
    manager->release();
    //Shutdown cAudio
    manager->shutDown();
    return 0;
}
