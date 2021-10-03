#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "log.h"

void sigterm_handler(int sig)
{
	exit(0);
}


int main(int argc, char **argv)
{
	char *a = "Test var";

	if (signal(SIGTERM, sigterm_handler) == SIG_ERR)
		print_current_error();

	switch(argv[1][0]) {
	case 'i': /* normal start/stop */
		break;
	case 's': /* To work with handler */
		sleep(15);
		break;
	case 'e': /* return an error state */
		sleep(15);
		exit(1);
	case 'b': /* crash */
		a[100] = 'j';
	}
	return 0;
}
