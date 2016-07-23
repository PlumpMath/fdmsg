
DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS += -Wall -Wextra -std=c89 -pedantic -fPIC -I .

OBJECTS := \
	connect.o \
	sendrecv.o

examples := \
	examples/main \
	examples/client \
	examples/server


all: libs
libs: libfdmsg.a libfdmsg.so
examples: $(examples)
clean:
	rm -f *.o */*.o *.so *.a $(examples)
	rm -f examples/client.c examples/server.c # Generated from README.md
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

examples/client.c examples/server.c: README.md
	./examples/extract_readme.sh


$(OBJECTS): fdmsg.h

.PHONY: all libs clean install install-static install-shared examples
