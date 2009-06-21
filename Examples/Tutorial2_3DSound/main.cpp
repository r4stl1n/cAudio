//****************************************************************
//cAudio 1.7.1 Tutorial 2
//Basic 3d Audio. Moving Audio source. Must be mono sound source
//****************************************************************

#include <iostream>
//Include IAudioManager so we can easily work with cAudio
#include "../../include/IAudioManager.h"
//Include IAudio so we can create cAudio objects
#include "../../include/IAudio.h"

using namespace std;

int main(int argc, char* argv[])
{
    //To make visual studio happy
	cAudio::IAudio* mysound;
    //Some fancy text
    cout <<"cAudio 1.7.1 Tutorial 2: 3dSound\n";
    //Hold audio source x position
    float x = 0;
    //Grap the cAudioManager
    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    //Init the cAudio Engine
    manager->init(argc,argv);
    manager->setListenerPos(0.0,0.0,0.0);
    //Create a IAudio object and load a sound from a file
    mysound = manager->createFromFile("bling","bling.ogg",true);
    //Set the IAudio Sound to play3d and loop
    //play3d takes 4 arguments play3d(toloop,x,y,z,strength)
    mysound->play3d(true,0.0,0.0,0.0,0.1);

    while(mysound->playing()){
    //Playback sound
	x+=0.0001;
    mysound->setPosition(x,0.0,0.0);
 	manager->update();

    }

    //Delete all IAudio sounds
    manager->release();
    //Shutdown cAudio
    manager->shutDown();
    return 0;
}
