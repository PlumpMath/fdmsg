

CFLAGS += -Wall -Wextra -std=c99 -pedantic -fPIC

OBJECTS := \
	connect.o \
	sendrecv.o



all: libs main

libs: libfdmsg.a libfdmsg.so

main: main.o libfdmsg.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.o libfdmsg.a
clean:
	rm -f *.o *.so *.a main

libfdmsg.a: $(OBJECTS)
	rm -f $@ ; ar crD $@ $(OBJECTS)

libfdmsg.so: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

install-libs: install-static install-shared
install-static: libfdmsg.a
	install -Dm644 $< $(DESTDIR)$(LIBDIR)/$<
install-shared: libfdmsg.so
	install -Dm755 $< $(DESTDIR)$(LIBDIR)/$<


$(OBJECTS): fdmsg.h

.PHONY: all libs clean
