//****************************************************************
//cAudio 2.0.0 Tutorial 4
//Audio Capture and playback
//****************************************************************

#include <iostream>
#include <string>
//Include cAudio.h so we can work wtih cAudio
#include "../../include/cAudio.h"

using namespace std;

const unsigned int CAPTURE_FREQUENCY = 22050;
const unsigned int CAPTURE_DURATION = 10;
const cAudio::AudioFormats CAPTURE_FORMAT = cAudio::EAF_16BIT_MONO;

int main(int argc, char* argv[])
{
    //Some fancy text
    cout << "cAudio 2.0.0 Tutorial 4: Capturing Audio. \n \n";

	std::string formatName;

	if(CAPTURE_FORMAT == cAudio::EAF_8BIT_MONO)
		formatName = "8 Bit Mono";
	else if(CAPTURE_FORMAT == cAudio::EAF_8BIT_STEREO)
		formatName = "8 Bit Stereo";
	else if(CAPTURE_FORMAT == cAudio::EAF_16BIT_MONO)
		formatName = "16 Bit Mono";
	else
		formatName = "16 Bit Stereo";

    //Create an Audio Manager with default settings
    cAudio::IAudioManager* manager = cAudio::createAudioManager();

	if(manager)
	{
		//! The capture interface can be gotten at any time, and can even be used completely seperate from the manager
		// Also it is possible to have more than one capture interface at the same time, useful for recording from multiple sources
		cAudio::IAudioCapture* capture = cAudio::createAudioCapture(false);
		if(capture)
		{
			bool captureReady = false;
			cout << "Capturing Supported: " << std::boolalpha << capture->isSupported() << "\n \n";
			if(capture->isSupported())
			{
				cout << "Available Capture Devices: \n";
				unsigned int deviceCount = capture->getAvailableDeviceCount();
				std::string defaultDeviceName = capture->getDefaultDeviceName();
				for(unsigned int i=0; i<deviceCount; ++i)
				{
					std::string deviceName = capture->getAvailableDeviceName(i);
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

				captureReady = capture->initialize(capture->getAvailableDeviceName(deviceSelection), CAPTURE_FREQUENCY, CAPTURE_FORMAT);
				cout << "Ready to capture audio: " << std::boolalpha << captureReady << "\n \n";

				//Quick math to figure out how large our data should be for the duration of the record time
				const unsigned int targetRecordSize = CAPTURE_FREQUENCY * CAPTURE_DURATION * capture->getSampleSize();
				
				//Print various information
				cout << "Capture Device: " << capture->getDeviceName() << ". \n";
				cout << "Capture Frequency: " << CAPTURE_FREQUENCY << " hertz. \n";
				cout << "Capture Duration: " << CAPTURE_DURATION << " seconds. \n";
				cout << "Capture Format: " << formatName << ". \n";
				cout << "Sample Size: " << capture->getSampleSize() << " bytes. \n";
				cout << "Target size of audio: " << targetRecordSize << " bytes. \n";
				cout << std::endl;

				unsigned int currentsize = 0;
				cout << "Starting capture... \n";
				if(capture->beginCapture())
				{
					while(currentsize < targetRecordSize)
					{
						currentsize = capture->getCurrentCapturedAudioSize();

						//Sleep for 1 ms to free some CPU
						cAudio::cAudioSleep(1);
					}
				}
				capture->stopCapture();
				cout << "Capture stopped... \n \n";

				//Grab the total size again, ensures we get ALL the audio data
				//Not completely necessary, as starting a capture again will clear the old audio data
				currentsize = capture->getCurrentCapturedAudioSize();

				char* buffer = new char[currentsize];
				cout << "Captured " << capture->getCapturedAudio(buffer, currentsize) << " bytes of audio data. \n \n";

				//Create a IAudio object and load a sound from a file
				cAudio::IAudioSource* mysound = manager->createFromRaw("sound1", buffer, currentsize, CAPTURE_FREQUENCY, CAPTURE_FORMAT);
				
				delete buffer;

				if(mysound)
				{
					cout << "Playing back captured audio... \n \n";
					mysound->setVolume(1.0);
					//Set the IAudio Sound to play2d and loop
					mysound->play2d(false);

					while(mysound->isPlaying())
					{
						//Sleep for 10 ms to free some CPU
						cAudio::cAudioSleep(10);
					}
				}
			}

			// Always remember to destroy the interface once you are done with it
			cAudio::destroyAudioCapture(capture);
			capture = 0x0;
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
