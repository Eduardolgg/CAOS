#include <stdio.h>
#include <pty.h>  // Para openpty
#include <utmp.h> // Para loggin_tty
#include <unistd.h>
#include <string.h>

//TODO: ALL, this code is wrong
int main(int argc, char **argv)
{
	int amaster;
	int aslave;

	openpty(&amaster, &aslave, NULL, NULL, NULL);
	printf("Amaster fd[%i], aslave fd[%i]\n", amaster, aslave);

	printf("Sending ls to pty\n");
	write(aslave, "ls", 2);

	printf("Reading pty output\n");

	int readed_bytes;
	char buffer[256];
	do {
		memset(buffer, '\0', 256);
		readed_bytes = read(amaster, buffer, 255);
		printf("%s", buffer);
	} while (readed_bytes == 255);
	printf("***PTY output end");

	return 0;
}
