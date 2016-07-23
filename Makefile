
DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS += -Wall -Wextra -std=c99 -pedantic -fPIC -I .

OBJECTS := \
	connect.o \
	sendrecv.o

examples := examples/main



all: libs
libs: libfdmsg.a libfdmsg.so
examples: $(examples)
clean:
	rm -f *.o */*.o *.so *.a $(examples)
install: install-static install-shared
install-static: libfdmsg.a
	install -Dm644 $< $(DESTDIR)$(LIBDIR)/$<
install-shared: libfdmsg.so
	install -Dm755 $< $(DESTDIR)$(LIBDIR)/$<

examples/%: examples/%.o libfdmsg.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< libfdmsg.a

libfdmsg.a: $(OBJECTS)
	rm -f $@ ; ar crD $@ $(OBJECTS)
libfdmsg.so: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)


$(OBJECTS): fdmsg.h

.PHONY: all libs clean install install-static install-shared examples
