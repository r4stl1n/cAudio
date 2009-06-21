SONAME=libcAudio.so
VERSION=1.7
DIR=../bin/Linux/x86 #x86_64

DEPEND=-lmikmod -logg -lopenal -lalut -lvorbis -lvorbisenc -lvorbisfile\
 -I../dependencies/Headers

OBJECT=cAudio.o cAudioManager.o cFileSource.o cListener.o cMemorySource.o cOggAudioDecoderFactory.o cOggDecoder.o cWavAudioDecoderFactory.o cWavDecoder.o

LINK_OPTIONS=-shared
COMPILE_OPTIONS=-fPIC

CC=g++

all: $(OBJECT)
	$(CC) -o $(SONAME) $(OBJECT) $(LINK_OPTIONS) $(DEPEND)

%.o: %.cpp
	$(CC) -c -o $@ $< $(COMPILE_OPTIONS)
