CC=gcc
CFLAGS=-Wall -g
BINS=libalarm.so libchild.so libxfsz.so libfpe.so libpipe.so libsegv.so

all: suite

suite: suite.so

suite.so: libalarm.c libchild.c libxfsz.c libfpe.c libpipe.c libsegv.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libalarm.c libchild.c libxfsz.c libfpe.c libpipe.c libsegv.c -lc

alarm: libalarm.so

libalarm.so: libalarm.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libalarm.c -lc

child: libchild.so

libchild.so: libchild.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libchild.c -lc

xfsz: libxfsz.so

libxfsz.so:
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libxfsz.c -lc

fpe: libfpe.so

libfpe.so: libfpe.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libfpe.c -lc

pipe: libpipe.so

libpipe.so: libpipe.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libpipe.c -lc

segv: libsegv.so

libsegv.so: libsegv.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libsegv.c -lc

clean:
	rm *.so 
