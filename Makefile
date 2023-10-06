CC = cc
CFLAGS = -Wall -Wextra

tval : tval.c
	$(CC) -o tval tval.c -lm -I.

install: tval
	cp tval /usr/local/bin/tval
	rm -f tval

clean:
	rm -f tval 