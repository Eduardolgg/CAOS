#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <pty.h>
#include <utmp.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

int main(int argc, char **argv) {
	int amaster,
	    w_status,
	    status;
	char buf[256];

	printf("++++ Calling forkpty\n");
	int pid = forkpty(&amaster, NULL, NULL, NULL);
	switch(pid) {
	case 0:
		execl("/bin/ls", "ls", "--color", "-l", (char*) NULL);
		exit(1);
	case -1:
		printf("Error creatingh child process");
		exit(1);
	default:
		w_status = waitpid(pid, &status, 0);
		if (w_status == -1) {
			printf("Error waiting for child process");
			exit(1);
		}
		printf("++++ Command output\n");
		memset(buf, '\0', 256);
		while(read(amaster, buf, sizeof(buf)-1) > 0) {
			printf("%s", buf);
			memset(buf, '\0', 256);
		}
		printf("++++ End PTY output\n");
	}
	return 0;
}
