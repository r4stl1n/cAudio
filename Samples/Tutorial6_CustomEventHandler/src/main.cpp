//****************************************************************
//cAudio 2.2.0 Tutorial 6
//Custom Event Handler
//****************************************************************

#include <iostream>
#include <string>

//Include cAudio.h so we can work wtih cAudio
#include "cAudio.h"
#ifdef CAUDIO_PLATFORM_WIN
#   define AUDIO_FILE(_soundName_) CAUDIO_MEDIA_ROOT##_soundName_
#else
#   define AUDIO_FILE(_soundName_) CAUDIO_MEDIA_ROOT#_soundName_
#endif

//Include the custom handler
#include "cTestHandler.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Some fancy text
    cout << "cAudio 2.1.0 Tutorial 6: Custom event handler. \n \n";

	//Create an uninitialized Audio Manager
    cAudio::IAudioManager* manager = cAudio::createAudioManager(false);
	
	//Make a pointer to our handler
	cTestHandler *handle = new cTestHandler;

	if(manager)
	{
		//Allow the user to choose a playback device
		cout << "\nAvailable Playback Devices: \n";
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
#ifdef CAUDIO_PLATFORM_WIN         
		cAudio::IAudioSource* mysound = manager->create("bling", AUDIO_FILE("cAudioTheme1.ogg"),true);
#else
		cAudio::IAudioSource* mysound = manager->create("bling", AUDIO_FILE(cAudioTheme1.ogg),true);        
#endif        
		mysound->registerEventHandler(handle);

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
		manager->releaseAllSources();
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
