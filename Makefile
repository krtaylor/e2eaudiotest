OBJECTS = testfreq.o utils_alsa.o

all: testfreq

testfreq: $(OBJECTS)
	gcc $(OBJECTS) -L/usr/lib/i386-linux-gnu -lasound -lfftw3 -o testfreq
	
clean:
	-rm $(OBJECTS) testfreq
