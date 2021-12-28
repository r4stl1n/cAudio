

#include <iostream>
#include <fluidlite.h>
#include "cAudio.h"

using namespace std;

#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 2 //4: Float Buffer   2: Signed Int Buffer
#define NUM_FRAMES SAMPLE_RATE
#define NUM_CHANNELS 2
#define NUM_SAMPLES (NUM_FRAMES * NUM_CHANNELS)
#define TIME_INTERVAL 1000000 //1500000:duration

int main()
{
	fluid_settings_t* settings = new_fluid_settings();
	fluid_synth_t* synth = new_fluid_synth(settings);
	int res = fluid_synth_sfload(synth, "sitar.sf2", 1);

	double dlength = (double)(SAMPLE_RATE * NUM_CHANNELS * SAMPLE_SIZE) * TIME_INTERVAL / 1000000;
	long length = (long)dlength;
	char* audio_buf = (char*)calloc(1, length);

	if (res <= 0)
	{
		printf("Could not soundfont.sf2\n");
		return -1;
	}

    //Some fancy text
    cout << "cAudio 2.3.0 Tutorial 2: Basic 3D Audio. \n \n";
    //Hold audio source x position
    float rot = 0;

    //Create an uninitialized Audio Manager
    cAudio::IAudioManager* audioMgr = cAudio::createAudioManager(false);

    if (audioMgr)
    {
        //Allow the user to choose a playback device
        cout << "\nAvailable Playback Devices: \n";

        cAudio::IAudioDeviceList* pDeviceList = cAudio::createAudioDeviceList();
        unsigned int deviceCount = pDeviceList->getDeviceCount();
        cAudio::cAudioString defaultDeviceName = pDeviceList->getDefaultDeviceName();
        for (unsigned int i = 0; i < deviceCount; ++i)
        {
            cAudio::cAudioString deviceName = pDeviceList->getDeviceName(i);
            if (deviceName.compare(defaultDeviceName) == 0)
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

        //Grab the listener object, which allows us to manipulate where "we" are in the world
        //It's useful to bind this to a camera if you are using a 3d graphics engine
        cAudio::IListener* listener = audioMgr->getListener();

		while (true)
		{
			int nKey = 60 + rand() % 30;
			fluid_synth_noteon(synth, 0, nKey, 127);
			fluid_synth_write_s16(synth, SAMPLE_RATE, audio_buf, 0, NUM_CHANNELS, audio_buf, 1, NUM_CHANNELS);
			fluid_synth_noteoff(synth, 0, 60);

			//Create a IAudio object and load a sound from a file
			//cAudio::IAudioSource* mysound = audioMgr->create("bling", "ec7_stereo.ogg", true);
			cAudio::IAudioSource* mysound = audioMgr->createFromRaw(
				"bling", audio_buf, length, SAMPLE_RATE, cAudio::EAF_16BIT_STEREO);

			//Set the IAudio Sound to play3d and loop
			//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
			if (mysound && listener)
			{
				listener->setPosition(cAudio::cVector3(0, 0, 0));
				mysound->play3d(cAudio::cVector3(0, 0, 0), 2.0f, true);
				mysound->setVolume(1.0f);
				mysound->setMinDistance(1.0f);
				mysound->setMaxAttenuationDistance(100.0f);

				//Play for 10 seconds
				const int ticksToPlay = 400;
				int currentTick = 0;
				int currentSecTick = 0;

				while (mysound->isPlaying() && currentTick < ticksToPlay)
				{
					//Figure out the location of our rotated sound
					rot += 0.1f * 0.017453293f;  //0.1 degrees a frame

					//Sound "starts" at x=5, y=0, z=0
					float x = 5.0f * cosf(rot) - 0.0f * sinf(rot);
					float z = 0.0f * cosf(rot) + 5.0f * sinf(rot);
					mysound->move(cAudio::cVector3(x, 0.0, z));

					++currentTick;

					if (currentTick / 1000 > currentSecTick)
					{
						++currentSecTick;
						std::cout << ".";
					}

					//Sleep for 1 ms to free some CPU
					cAudio::cAudioSleep(1);
				}
			}
			//audioMgr->releaseAllSources();
		}

		std::cout << std::endl;

		cAudio::destroyAudioManager(audioMgr);
	}
	else
	{
		std::cout << "Failed to create audio playback manager. \n";
	}

	free(audio_buf);

	std::cout << "Press any key to quit \n";
}
