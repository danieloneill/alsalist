alsalist: alsalist.c
	gcc -o alsalist alsalist.c `pkg-config --cflags --libs alsa`

all: alsalist

clean:
	rm alsalist

install: alsalist
	install -c alsalist /usr/local/bin/
