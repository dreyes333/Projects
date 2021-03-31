all: assign6.c
	gcc -g assign6.c -lpcap -ldumbnet -o assign6

clean:
	$(RM) assign6
