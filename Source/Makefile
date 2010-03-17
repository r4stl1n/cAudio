VERSION=2.1.0
SONAME=libcAudio.$(VERSION).so
DIR=../bin/Linux/x86 #x86_64

DEPEND=-logg -lopenal -lvorbis -lvorbisfile -ldl\
 -I../dependencies/Headers

STATICDEPEND=/usr/lib/libogg.a /usr/lib/libopenal.so /usr/lib/libvorbis.a /usr/lib/libvorbisenc.a /usr/lib/libvorbisfile.a /usr/lib/libdl.a

OBJECT= cAudioCapture.o cAudioEffects.o cAudioManager.o cAudioSleep.o cAudioSource.o cConsoleLogReceiver.o cEffect.o cFileSource.o cFilter.o cFileLogReceiver.o cListener.o cLogger.o cMemorySource.o cMutex.o cOggAudioDecoderFactory.o cOggDecoder.o cPluginManager.o cRawDecoder.o cThread.o cWavAudioDecoderFactory.o cWavDecoder.o cMemoryTracker.o cStandardMemoryProvider.o cAudioMemory.o

LINK_OPTIONS=-shared 
COMPILE_OPTIONS=-fPIC
CC=g++

all: shared

shared: $(OBJECT)
	$(CC) -o $(SONAME) $(OBJECT) $(LINK_OPTIONS) $(DEPEND) 
	cp libcAudio.$(VERSION).so ../lib/linux-x86
	if test -f ../bin/linux-x86/libcAudio.so;\
	then rm ../bin/linux-x86/libcAudio.so;\
	ln -s ../../lib/linux-x86/libcAudio.$(VERSION).so ../bin/linux-x86/libcAudio.so;\
	else ln -s ../../lib/linux-x86/libcAudio.$(VERSION).so ../bin/linux-x86/libcAudio.so;\
	fi

static: $(OBJECT)
	ar cr libcAudio.$(VERSION).a *.o $(STATICDEPEND)
	ranlib libcAudio.$(VERSION).a 
	cp libcAudio.$(VERSION).a ../lib/linux-x86
	if test -f ../bin/linux-x86/libcAudio.a;\
	then rm ../bin/linux-x86/libcAudio.a;\
	ln -s ../../lib/linux-x86/libcAudio.$(VERSION).a ../bin/linux-x86/libcAudio.a;\
	else ln -s ../../lib/linux-x86/libcAudio.$(VERSION).a ../bin/linux-x86/libcAudio.a;\
	fi;

install: $(OBJECT)
	if test -d /usr/local/lib/cAudio$(VERSION); \
	then sudo cp libcAudio.$(VERSION).so /usr/local/lib/cAudio$(VERSION); \
	else mkdir /usr/local/lib/cAudio$(VERSION);  \
	sudo cp libcAudio.$(VERSION).so /usr/local/lib/cAudio$(VERSION);\
	fi
	
	if test -d /usr/local/include/cAudio; \
	then sudo cp -r ../include/* /usr/local/include/cAudio; \
	else mkdir /usr/local/include/cAudio; \
	sudo cp -r ../include/* /usr/local/include/cAudio;\
	fi

	if test -f /usr/local/lib/libcAudio.so;\
	then sudo rm /usr/local/lib/libcAudio.so;\
	sudo ln -s /usr/local/lib/cAudio$(VERSION)/libcAudio.$(VERSION).so /usr/local/lib/libcAudio.so;\
	else sudo ln -s /usr/local/lib/cAudio$(VERSION)/libcAudio.$(VERSION).so /usr/local/lib/libcAudio.so;\
	fi
	
	ldconfig


installStatic: $(OBJECT)
	if test -d /usr/local/lib/cAudio$(VERSION); \
	then sudo cp libcAudio.$(VERSION).a /usr/lib/cAudio$(VERSION); \
	else mkdir /usr/local/lib/cAudio$(VERSION);  \
	sudo cp libcAudio.$(VERSION).a /usr/local/lib/cAudio$(VERSION);\
	fi
	
	if test -d /usr/local/include/cAudio; \
	then sudo cp -r ../include/* /usr/local/include/cAudio; \
	else mkdir /usr/local/include/cAudio; \
	sudo cp -r ../include/* /usr/local/include/cAudio;\
	fi
	
	if test -f /usr/local/lib/libcAudio.a;\
	then sudo rm /usr/local/lib/libcAudio.a;\
	sudo ln -s /usr/local/lib/cAudio$(VERSION)/libcAudio.$(VERSION).a /usr/local/lib/libcAudio.a;\
	else sudo ln -s /usr/local/lib/cAudio$(VERSION)/libcAudio.$(VERSION).a /usr/local/lib/libcAudio.a;\
	fi

	ldconfig

buildExamples: $(OBJECT)
	cd ../Examples;\
	./buildAll.sh

clean:
	cd ../Examples
	rm -f *.o *.so lib*.so libcAudio.$(VERSION).a *~

%.o: %.cpp
	$(CC) -c -o $@ $< $(COMPILE_OPTIONS)

