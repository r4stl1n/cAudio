//****************************************************************
//cAudio 2.1.0 Stress Test
//Used to push cAudio to its limits
//****************************************************************

#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>

//Include IAudioManager so we can easily work with cAudio
#include "../../include/cAudio.h"

using namespace std;

#define MAXAUDIOSOURCES 64
#define TESTDURATION 10

cAudio::IAudioSource* AudioSources[MAXAUDIOSOURCES];

inline float getRandomFloat(float fMin, float fMax)
{
	float fUnit = float(rand( )) / RAND_MAX;
	float fDiff = fMax - fMin;

	return fMin + fUnit * fDiff;
}

int main(int argc, char* argv[])
{
    //Some fancy text
    cout << "cAudio 2.1.0 Stress Test \n \n";
	srand(time(NULL));

	unsigned int startTick = clock();

	//Create an uninitialized Audio Manager
    cAudio::IAudioManager* manager = cAudio::createAudioManager(false);

	//Limit the severity of log messages to just warnings, for best performance
	cAudio::getLogger()->setLogLevel(cAudio::ELL_WARNING);

	if(manager)
	{
		//Allow the user to choose a playback device
		/*cout << "Available Playback Devices: \n";
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
		cout << std::endl;*/

		//Initialize the manager with the user settings
		//manager->initialize(manager->getAvailableDeviceName(deviceSelection));
		manager->initialize(NULL);

		//Grab the listener object, which allows us to manipulate where "we" are in the world
		//It's useful to bind this to a camera if you are using a 3d graphics engine
		cAudio::IListener* listener = manager->getListener();

		cout << "Creating " << MAXAUDIOSOURCES << " sources. \n";
		unsigned int startTick = clock();
		for(unsigned int i=0; i<MAXAUDIOSOURCES; ++i)
		{
			AudioSources[i] = manager->create("bling", "../../media/bling.ogg", false);
		}
		unsigned int endTick = clock();
		unsigned int elaspedTicks = endTick - startTick;
		cout << "Took " << (float)elaspedTicks/(float)CLOCKS_PER_SEC << " seconds. \n \n";

		cout << "Telling " << MAXAUDIOSOURCES << " sources to play (in 3D). \n";
		startTick = clock();
		for(unsigned int i=0; i<MAXAUDIOSOURCES; ++i)
		{
			if(AudioSources[i])
			{
				AudioSources[i]->play3d(cAudio::cVector3(getRandomFloat(-100,100),getRandomFloat(-100,100),getRandomFloat(-100,100)),2.0f,true);
				AudioSources[i]->setMinDistance(10.0f);
				AudioSources[i]->setMaxDistance(200.0f);
			}
		}
		endTick = clock();
		elaspedTicks = endTick - startTick;
		cout << "Took " << (float)elaspedTicks/(float)CLOCKS_PER_SEC << " seconds. \n \n";

		cout << "Playing and moving all sources (" << MAXAUDIOSOURCES << ") for " << TESTDURATION << " seconds. \n";
		unsigned int currentTick = clock();
		unsigned int currentCycle = 0;
		while(currentTick < (TESTDURATION*CLOCKS_PER_SEC))
		{
			for(unsigned int i=0; i<MAXAUDIOSOURCES; ++i)
			{
				if(AudioSources[i])
				{
					AudioSources[i]->move(AudioSources[i]->getPosition() + cAudio::cVector3(getRandomFloat(-10,10),getRandomFloat(-10,10),getRandomFloat(-10,10)));
					++currentCycle;
				}
			}
			currentTick = clock();
		}
		cout << "Did " << currentCycle << " source moves in " << TESTDURATION << " seconds. \n";
		cout << "Average Moves Per Second: " << (((float)currentCycle)/(float)TESTDURATION) << " \n";
		cout << "Average Frames (All audio sources moved) Per Second: " << (((float)currentCycle/(float)MAXAUDIOSOURCES)/(float)TESTDURATION) << " \n \n";

		std::cout << std::endl;

		cout << "Deleting " << MAXAUDIOSOURCES << " sources. \n";
		startTick = clock();
		//Delete all IAudio sounds
		manager->releaseAllSources();
		endTick = clock();
		elaspedTicks = endTick - startTick;
		cout << "Took " << (float)elaspedTicks/(float)CLOCKS_PER_SEC << " seconds. \n \n";

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
	//std::cin.get();

    return 0;
}
