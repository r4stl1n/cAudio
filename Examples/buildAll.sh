#!/bin/bash
cd Tutorial1_2DSound
g++ main.cpp -o ../../bin/linux-x86/2DSound -lcAudio
cd ../
cd Tutorial2_3DSound
g++ main.cpp -o ../../bin/linux-x86/3DSound -lcAudio
cd ../
cd Tutorial3_MemoryPlayback
g++ main.cpp -o ../../bin/linux-x86/MemoryPlayback -lcAudio
cd ../
