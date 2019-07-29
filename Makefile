CC=gcc
CFLAGS=-Wall -g
#BINS=signalsuite libalarm.so signals
BINS=libalarm.so


all: $(BINS)

libalarm.o: libalarm.c alarm.h
	$(CC) $(CFLAGS) -c libalarm.c

libalarm.so: libalarm.c alarm.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libalarm.c -lc

#signalsuite: signalsuite.c libalarm.o
#	$(CC) $(CFLAGS) -o $@ $^

#signals: signalsuite.c
#	$(CC) $(CFLAGS) -o $@ $^ -L. -lalarm

clean:
	rm *.so 
