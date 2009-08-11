#include <iostream>
#include <string>
//Include IAudioManager so we can easily work with cAudio
#include "../../include/IAudioManager.h"
//Include IAudio so we can create cAudio objects
#include "../../include/IAudio.h"

using namespace std;

#define CAPTURE_FREQUENCY 22050
#define CAPTURE_DURATION 10
#define CAPTURE_FORMAT cAudio::EAF_16BIT_MONO

int main(int argc, char* argv[])
{
    //To make visual studio happy
    cAudio::IAudio* mysound = NULL;

    //Some fancy text
    cout << "cAudio 1.7.1 Tutorial 4: Capturing Audio \n \n";

	std::string formatName;

	if(CAPTURE_FORMAT == cAudio::EAF_8BIT_MONO)
		formatName = "8 Bit Mono";
	else if(CAPTURE_FORMAT == cAudio::EAF_8BIT_STEREO)
		formatName = "8 Bit Stereo";
	else if(CAPTURE_FORMAT == cAudio::EAF_16BIT_MONO)
		formatName = "16 Bit Mono";
	else
		formatName = "16 Bit Stereo";

    //Grap the cAudioManager
    cAudio::IAudioManager* manager = cAudio::getAudioManager();
    //Init the cAudio Engine
    manager->init(argc,argv);

	//! The capture interface should be grabbed after the manager has been initialized
	cAudio::IAudioCapture* capture = manager->getAudioCapture();
	bool captureReady = false;
	cout << "Capturing Supported: " << std::boolalpha << capture->isSupported() << "\n";
	if(capture->isSupported())
	{
		captureReady = capture->initialize(CAPTURE_FREQUENCY, CAPTURE_FORMAT);
		cout << "Ready to capture audio: " << std::boolalpha << captureReady << "\n \n";

		//Quick math to figure out how large our data should be for the duration of the record time
		const int targetRecordSize = CAPTURE_FREQUENCY * CAPTURE_DURATION * capture->getSampleSize();
		cout << "Capture Frequency: " << CAPTURE_FREQUENCY << "\n";
		cout << "Capture Duration: " << CAPTURE_DURATION << "\n";
		cout << "Capture Format: " << formatName << "\n";
		cout << "Sample Size: " << capture->getSampleSize() << "\n";
		cout << "Total size of audio: " << targetRecordSize << "\n";

		cout << std::endl;

		int currentsize = 0;
		cout << "Starting capture... \n";
		if(capture->beginCapture())
		{
			while(currentsize < targetRecordSize)
			{
				currentsize = capture->getCurrentCapturedAudioSize();
				//Run the main update loop to keep audio data flowing in
				manager->update();
			}
		}
		capture->stopCapture();
		cout << "Capture stopped... \n \n";

		char* buffer = new char[currentsize];
		capture->getCapturedAudio(buffer, currentsize);

		//Create a IAudio object and load a sound from a file
		mysound = manager->createFromRaw("sound1", buffer, currentsize, CAPTURE_FREQUENCY, CAPTURE_FORMAT);
		
		delete buffer;

		if(mysound)
		{
			cout << "Playing back captured audio... \n \n";
			mysound->setVolume(1.0);
			//Set the IAudio Sound to play2d and loop
			mysound->play2d(false);

			while(mysound->playing())
			{
				//Playback sound
 				manager->update();
			}
		}
	}

    //Delete all IAudio sounds
    manager->release();
    //Shutdown cAudio
    manager->shutDown();

	std::cout << "Press any key to quit \n";
	std::cin.get();

    return 0;
}
