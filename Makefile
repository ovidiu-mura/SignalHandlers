CC=gcc
CFLAGS=-g
BINS=libalarm.so libchild.so libxfsz.so libfpe.so libpipe.so libsegv.so libstop.so libcont.so

all: suite

sep: $(BINS)

suite: suite.so

suite.so: libalarm.c libchild.c libxfsz.c libfpe.c libpipe.c libsegv.c libstop.c libcont.c libusr.c libint.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libalarm.c libchild.c libxfsz.c libfpe.c libpipe.c libsegv.c libstop.c libcont.c libusr.c libint.c -lc

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

stop: libstop.so

libstop.so: libstop.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libstop.c -lc

cont: libcont.so

libcont.so: libcont.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libcont.c -lc

usr: libusr.so

libusr.so: libusr.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libusr.c -lc

int: libint.so

libint.so: libint.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libint.c -lc


clean:
	rm *.so 
