CC=gcc
CFLAGS=-Wall
LDFLAGS=-L../../Source -lcAudio

OBJECT=main.o cMP3Decoder.o mpaudec/bits.c mpaudec/mpaudec.c
LINK_OPTIONS=-shared -DCAUDIO_PLATFORM_LINUX

all:	$(OBJECT)
			$(CC) -o cAp_MP3Decoder.so $(OBJECT) $(LINK_OPTIONS) $(LDFLAGS)