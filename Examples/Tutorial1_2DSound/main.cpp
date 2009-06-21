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
    cout <<"cAudio 1.7.1 Tutorial 1: 2DSound\n";

    //Grap the cAudioManager
    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    //Init the cAudio Engine
    manager->init(argc,argv);
    //Create a IAudio object and load a sound from a file
    mysound = manager->createFromFile("bling","../../bin/bling.ogg",true);
    //Set the IAudio Sound to play2d and loop
    mysound->play2d(true);

    while(mysound->playing()){
        //Playback sound
 	manager->update();

    }

    //Delete all IAudio sounds
    manager->release();
    //Shutdown cAudio
    manager->shutDown();
    return 0;
}
