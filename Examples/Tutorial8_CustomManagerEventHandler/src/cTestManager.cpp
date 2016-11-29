//****************************************************************
//cAudio 2.3.0 Tutorial 8
//Custom Manager Event Handler
//****************************************************************

#include "cTestManager.h"
#include <iostream>

using namespace std;

void cTestManager::onUpdate()
{
	//We comment this out because this will be constantly 
	//called as the thread is updated
	//std::cout << "Custom Threaded Manager Event Handler : update called\n";
    
    clock_t t = clock();
    if (mysoundFading)
        fadeAccumulator -= (t - fadeClockLastVal);
    else
        fadeAccumulator += (t - fadeClockLastVal);
	fadeClockLastVal = t;
	float newVolume = float(fadeAccumulator) / CLOCKS_PER_SEC * 4.f;
    mysound->setVolume(newVolume);
    if (newVolume < 0.01f && mysoundFading)
    {
        std::cout  <<"fading in...\n";
        mysoundFading = false;
    }
    if (newVolume > 0.99f && !mysoundFading)
    {
        std::cout  <<"fading out...\n";
        mysoundFading = true;
    }
    //std::cout  <<"current volume is :" << newVolume << "\n";
}

cTestManager::~cTestManager()
{
    audioMgr->shutDown();
	cAudio::destroyAudioManager(audioMgr);   
}

cTestManager::cTestManager()
{
	//Create an uninitialized Audio Manager
    audioMgr = cAudio::createAudioManager(false);
    mysoundFading = false;

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

		//Initialize the manager with the user settings
		audioMgr->initialize(pDeviceList->getDeviceName(deviceSelection).c_str());
		CAUDIO_DELETE pDeviceList;
		pDeviceList = 0;

        //register for callbacks
        audioMgr->registerEventHandler(this);
    
    	//Create a IAudio object and load a sound from a file
		mysound = audioMgr->create("song", "../Media/cAudioTheme1.ogg",true);

		if(mysound)
		{
			mysound->setVolume(0.0);
			//Set the IAudio Sound to play2d and loop
			mysound->play2d(true);
		}
        else
        {
            cout << "Failed to load audio source file. \n";
        }
    }
	else
	{
		std::cout << "Failed to create audio playback manager. \n";
	}
    
}

