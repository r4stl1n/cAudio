#!/bin/bash
cd Tutorial1_2DSound
g++ main.cpp -o ../../bin/linux-x86/Tutorial1_2DSound -lcAudio
cd ../
cd Tutorial2_3DSound
g++ main.cpp -o ../../bin/linux-x86/Tutorial2_3DSound -lcAudio
cd ../
cd Tutorial3_MemoryPlayback
g++ main.cpp -o ../../bin/linux-x86/Tutorial3_MemoryPlayback -lcAudio
cd ../
cd Tutorial4_AudioCapture
g++ main.cpp -o ../../bin/linux-x86/Tutorial4_AudioCapture -lcAudio
cd ../
cd Tutorial5_AudioEffects
g++ main.cpp -o ../../bin/linux-x86/Tutorial5_AudioEffects -lcAudio
cd ../ 
cd Tutorial6_CustomEventHandler
g++ main.cpp cTestHandler.cpp -o ../../bin/linux-x86/Tutorial6_CustomEventHandler -lcAudio
cd ../ 
cd Tutorial7_CustomLogReceiver
g++ main.cpp cTestLogReceiver.cpp -o ../../bin/linux-x86/Tutorial7_CustomLogReceiver -lcAudio
cd ../ 