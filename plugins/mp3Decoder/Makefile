CC=gcc
CFLAGS=-Wall -c -fPIC
LDFLAGS=-L../../Source -lcAudio

OBJECT=main.o cMP3Decoder.o bits.o mpaudec.o
LINK_OPTIONS=-shared -DCAUDIO_PLATFORM_LINUX

all:	main.o cMP3Decoder.o bits.o mpaudec.o
			$(CC) -o cAp_MP3Decoder.so $(OBJECT) $(LINK_OPTIONS) $(LDFLAGS)
			
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(LDFLAGS)

cMP3Decoder.o: cMP3Decoder.cpp
	$(CC) $(CFLAGS) cMP3Decoder.cpp $(LDFLAGS)

bits.o: mpaudec/bits.c
	$(CC) $(CFLAGS) mpaudec/bits.c
	
mpaudec.o: mpaudec/mpaudec.c
	$(CC) $(CFLAGS) mpaudec/mpaudec.c