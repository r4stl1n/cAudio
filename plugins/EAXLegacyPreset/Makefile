CC=g++
CFLAGS=-Wall -c -fPIC
LDFLAGS=-L../../Source -lcAudio

OBJECT=main.o
LINK_OPTIONS=-shared -DCAUDIO_PLATFORM_LINUX

all:	main.o
			$(CC) -o cAp_EAX_LegacyPreset.so $(OBJECT) $(LINK_OPTIONS) $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(LDFLAGS)