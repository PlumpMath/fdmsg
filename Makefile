

CFLAGS += -Wall -Wextra -std=c89 -pedantic

OBJECTS := \
	connect.o \
	sendrecv.o \
	main.o


all: main

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
clean:
	rm -f *.o main


$(OBJECTS): fdmsg.h
