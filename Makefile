PROJECT=main
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LFLAGS=-lm -lpthread
CC=arm-linux-gnueabihf-gcc
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))


all: $(PROJECT)
%.o: %.c
	$(CC) -c $< $(CFLAGS)
	
$(PROJECT): ${OBJECTS}
	$(CC) ${OBJECTS} -o $@ $(LFLAGS)

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROJECT) $(DESTDIR)
uninstall:
	sudo rm $(DESTDIR)/$(PROJECT)
clean:
	rm -f *.o $(PROJECT)
