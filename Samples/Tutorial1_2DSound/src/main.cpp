//****************************************************************
//cAudio 2.2.0 Tutorial 1
//Basic 2d Audio
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
using namespace std;

int main(int argc, char* argv[]) {
    //Some fancy text
    cout << "cAudio 2.2.0 Tutorial 1: Basic 2D Audio. \n \n";

	//Create an uninitialized Audio Manager
    cAudio::IAudioManager* audioMgr = cAudio::createAudioManager(false);

	if(audioMgr)
	{
		//Allow the user to choose a playback device
		cout << "\nAvailable Playback Devices: \n";
		cAudio::IAudioDeviceList* pDeviceList = cAudio::createAudioDeviceList();
		unsigned int deviceCount = pDeviceList->getDeviceCount();
		cAudio::cAudioString defaultDeviceName = pDeviceList->getDefaultDeviceName();
		for(unsigned int i=0; i<deviceCount; ++i)
		{
			cAudio::cAudioString deviceName = pDeviceList->getDeviceName(i);
			if(deviceName.compare(defaultDeviceName) == 0)
				cout << i << "): " << deviceName.c_str() << " [DEFAULT] \n";
			else
				cout << i << "): " << deviceName.c_str() << " \n";
		}
		cout << std::endl;
		cout << "Choose a device by number: ";
		unsigned int deviceSelection = 0;
		cin >> deviceSelection;
		cout << std::endl;

		//Initialize the Audio Manager with the user settings
		audioMgr->initialize(pDeviceList->getDeviceName(deviceSelection).c_str());
		CAUDIO_DELETE pDeviceList;
		pDeviceList = 0;

		//Create a IAudio object and load a sound from a file
#ifdef CAUDIO_PLATFORM_WIN         
		cAudio::IAudioSource* mysound = audioMgr->create("bling", AUDIO_FILE("cAudioTheme1.ogg"),true);
#else
		cAudio::IAudioSource* mysound = audioMgr->create("bling", AUDIO_FILE(cAudioTheme1.ogg),true);        
#endif 

		for (size_t i=0; i<10; i++)
		{
#ifdef CAUDIO_PLATFORM_WIN   
			audioMgr->play2D(AUDIO_FILE("bling.ogg"));
#else
			audioMgr->play2D(AUDIO_FILE(bling.ogg));
#endif 
		}
		
		if(mysound)
		{
			mysound->setVolume(0.5);
			//Set the IAudio Sound to play2d and loop
			mysound->play2d(false);

			//Wait for the sound to finish playing
			while(mysound->isPlaying())
				cAudio::cAudioSleep(10);

		}

		cAudio::destroyAudioManager(audioMgr);
	}
	else
	{
		std::cout << "Failed to create audio playback audioMgr. \n";
	}

	std::cout << "Press any key to quit \n";
	std::cin.get();
	std::cin.get();

    return 0;
}
