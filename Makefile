all:
	gcc -o alsalist alsalist.c `pkg-config --cflags --libs alsa`
