#include <iostream>
#include <string>

//Include IAudioManager so we can easily work with cAudio
#include "../../include/IAudioManager.h"
//Include IAudio so we can create cAudio objects
#include "../../include/IAudio.h"
//Include our version of Sleep to free CPU usage
#include "../../include/cAudioSleep.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Some fancy text
    cout << "cAudio 1.7.1 Tutorial 1: 2DSound \n \n";

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

		//Create a IAudio object and load a sound from a file
		cAudio::IAudio* mysound = manager->createFromFile("bling","../../media/cAudioTheme1.ogg",true);

		if(mysound)
		{
			mysound->setVolume(0.5);
			//Set the IAudio Sound to play2d and loop
			mysound->play2d(false);

			//Wait for the sound to finish playing
			while(mysound->isPlaying())
				cAudio::cAudioSleep(10);
		}

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
