VERSION=2.0.0
SONAME=libcAudio.$(VERSION).so
DIR=../bin/Linux/x86 #x86_64/home/wildicv/Documents/cAudio/Trunk/Source

DEPEND=-logg -lopenal -lvorbis -lvorbisenc -lvorbisfile\
 -I../dependencies/Headers

OBJECT= cAudioCapture.o cAudioEffects.o cAudioManager.o cAudioSleep.o cAudioSource.o cConsoleLogReceiver.o cEffect.o cFileSource.o cFilter.o cFileLogReceiver.o cListener.o cLogger.o cMemorySource.o cMutex.o cOggAudioDecoderFactory.o cOggDecoder.o cPluginManager.o cRawDecoder.o cThread.o cWavAudioDecoderFactory.o cWavDecoder.o 

LINK_OPTIONS=-shared 
COMPILE_OPTIONS=-fPIC
CC=g++

all: shared

shared: $(OBJECT)
	$(CC) -o $(SONAME) $(OBJECT) $(LINK_OPTIONS) $(DEPEND)
	cp libcAudio.2.0.0.so ../bin/linux-x86/
	ln -s ../bin/linux-x86/libcAudio.2.0.0.so ../bin/linux-x86/libcAudio.so
	
static: $(OBJECT)
	ar cr libcAudio.$(VERSION).a *.o
	ranlib libcAudio.$(VERSION).a
	cp libcAudio.2.0.0.a ../bin/linux-x86/
	ln -s ../bin/linux-x86/libcAudio.2.0.0.a ../bin/linux-x86/libcAudio.a
	
install: $(OBJECT)
	if test -d /usr/local/lib/cAudio2.0.0; \
	then sudo cp libcAudio.$(VERSION).a /usr/local/lib/cAudio2.0.0; \
	else mkdir /usr/local/lib/cAudio2.0.0;  \
	sudo cp libcAudio.$(VERSION).a /us/home/wildicv/Documents/cAudio/Trunk/Sourcer/local/lib/cAudio2.0.0;\
	fi
	
	if test -d /usr/local/include/cAudio2.0.0; \
	then sudo cp -r ../include/* /usr/local/include/cAudio2.0.0; \
	else mkdir /usr/local/include/cAudio2.0.0; \
	sudo cp -r ../include/* /usr/local/include/cAudio2.0.0;\
	fi
	sudo rm /usr/local/lib/libcAudio.a
	sudo ln -s /usr/local/lib/cAudio2.0.0/libcAudio.2.0.0.a /usr/local/lib/libcAudio.a
	ldconfig

installShared: $(OBJECT)
	if test -d /usr/local/lib/cAudio2.0.0; \
	then sudo cp libcAudio.$(VERSION).so /usr/local/lib/cAudio2.0.0; \
	else mkdir /usr/local/lib/cAudio2.0.0;  \
	sudo cp libcAudio.$(VERSION).so /usr/local/lib/cAudio2.0.0;\
	fi
	
	if test -d /usr/local/include/cAudio2.0.0; \
	then sudo cp -r ../include/* /usr/local/include/cAudio2.0.0; \
	else mkdir /usr/local/include/cAudio2.0.0; \
	sudo cp -r ../include/* /usr/local/include/cAudio2.0.0;\
	fi
	sudo rm /usr/local/lib/libcAudio.so
	sudo ln -s /usr/local/lib/cAudio2.0.0/libcAudio.2.0.0.so /usr/local/lib/libcAudio.so
	ldconfig

buildExamples: $(OBJECT)
	cd ../Examples;\
	./buildAll.sh

clean:
	cd ../Examples
	rm -f *.o *.so lib*.so libcAudio.$(VERSION).a *~

%.o: %.cpp
	$(CC) -c -o $@ $< $(COMPILE_OPTIONS)

