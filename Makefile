
DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS += -Wall -Wextra -std=c99 -pedantic -fPIC -I .

OBJECTS := \
	connect.o \
	sendrecv.o



all: libs examples/main

libs: libfdmsg.a libfdmsg.so

examples/main: examples/main.o libfdmsg.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ examples/main.o libfdmsg.a
clean:
	rm -f *.o *.so *.a main

libfdmsg.a: $(OBJECTS)
	rm -f $@ ; ar crD $@ $(OBJECTS)

libfdmsg.so: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

install: install-static install-shared
install-static: libfdmsg.a
	install -Dm644 $< $(DESTDIR)$(LIBDIR)/$<
install-shared: libfdmsg.so
	install -Dm755 $< $(DESTDIR)$(LIBDIR)/$<


$(OBJECTS): fdmsg.h

.PHONY: all libs clean install install-static install-shared
