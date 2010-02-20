SONAME=libcAudio.so
VERSION=2.0
DIR=../bin/Linux/x86 #x86_64

DEPEND=-logg -lopenal -lvorbis -lvorbisenc -lvorbisfile\
 -I../dependencies/Headers

OBJECT= cAudioCapture.o cAudioEffects.o cAudioManager.o cAudioSleep.o cAudioSource.o cConsoleLogReceiver.o cEffect.o cFileSource.o cFilter.o cFileLogReceiver.o cListener.o cLogger.o cMemorySource.o cMutex.o cOggAudioDecoderFactory.o cOggDecoder.o cPluginManager.o cRawDecoder.o cThread.o cWavAudioDecoderFactory.o cWavDecoder.o 

LINK_OPTIONS=-shared
COMPILE_OPTIONS=-fPIC

CC=g++

all: $(OBJECT)
	$(CC) -o $(SONAME) $(OBJECT) $(LINK_OPTIONS) $(DEPEND)

%.o: %.cpp
	$(CC) -c -o $@ $< $(COMPILE_OPTIONS)
