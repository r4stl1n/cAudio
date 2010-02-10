//****************************************************************
//cAudio 2.0 Tutorial 2
//Basic 3d Audio. Moving Audio source. Must be mono sound source
//****************************************************************

#include <iostream>
#include <string>
#include <math.h>

//Include cAudio.h so we can work wtih cAudio
#include "../../include/cAudio.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Some fancy text
    cout << "cAudio 1.7.1 Tutorial 2: 3dSound \n \n";
    //Hold audio source x position
    float rot = 0;

	//Create an uninitialized Audio Manager
    cAudio::IAudioManager* manager = cAudio::createAudioManager(false);

	if(manager)
	{
		//Allow the user to choose a playback device
		cout << "Available Playback Devices: \n";
		unsigned int deviceCount = manager->getAvailableDeviceCount();
		std::string defaultDeviceName = manager->getDefaultDeviceName();
		for(unsigned int i=0; i<deviceCount; ++i)
		{
			std::string deviceName = manager->getAvailableDeviceName(i);
			if(deviceName.compare(defaultDeviceName) == 0)
				cout << i << "): " << deviceName << " [DEFAULT] \n";
			else
				cout << i << "): " << deviceName << " \n";
		}
		cout << std::endl;
		cout << "Choose a device by number: ";
		unsigned int deviceSelection = 0;
		cin >> deviceSelection;
		cout << std::endl;

		//Initialize the manager with the user settings
		manager->initialize(manager->getAvailableDeviceName(deviceSelection));

		//Grab the listener object, which allows us to manipulate where "we" are in the world
		//It's useful to bind this to a camera if you are using a 3d graphics engine
		cAudio::IListener* listener = manager->getListener();

		//Create a IAudio object and load a sound from a file
		cAudio::IAudioSource* mysound = manager->createFromFile("bling", "../../media/bling.ogg", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if(mysound && listener)
		{
			listener->setPosition(cAudio::cVector3(0,0,0));
			mysound->play3d(cAudio::cVector3(0,0,0),2.0f,true);
			mysound->setVolume(1.0f);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);

			//Play for 10 seconds
			const int ticksToPlay = 10000;
			int currentTick = 0;
			int currentSecTick = 0;

			while(mysound->isPlaying() && currentTick < ticksToPlay)
			{
				//Figure out the location of our rotated sound
				rot+=0.1f * 0.017453293f;  //0.1 degrees a frame

				//Sound "starts" at x=5, y=0, z=0
				float x = 5.0f * cosf(rot) - 0.0f * sinf(rot);
				float z = 0.0f * cosf(rot) + 5.0f * sinf(rot);
				mysound->move(cAudio::cVector3(x,0.0,z));

				++currentTick;

				if(currentTick/1000 > currentSecTick)
				{
					++currentSecTick;
					std::cout << ".";
				}

				//Sleep for 1 ms to free some CPU
				cAudio::cAudioSleep(1);
			}
		}

		std::cout << std::endl;

		//Delete all IAudio sounds
		manager->release();
		//Shutdown cAudio
		manager->shutDown();

		cAudio::destroyAudioManager(manager);
	}
	else
	{
		std::cout << "Failed to create audio playback manager. \n";
	}

	std::cout << "Press any key to quit \n";
	std::cin.get();
	std::cin.get();

    return 0;
}
