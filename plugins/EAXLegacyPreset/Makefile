CC=g++
CFLAGS=-Wall
LDFLAGS=-L../../Source -lcAudio

OBJECT=main.o
LINK_OPTIONS=-shared -DCAUDIO_PLATFORM_LINUX

all:	$(OBJECT)
			$(CC) -o cAp_EAX_LegacyPreset.so $(OBJECT) $(LINK_OPTIONS) $(LDFLAGS)