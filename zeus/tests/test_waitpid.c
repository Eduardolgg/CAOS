#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#include "log.h"
#include "processes.h"

#define DELAY sleep(2)

void *send_signals_to_child(void *pid)
{
	pid_t pid_ = *((pid_t *) pid);

 	DELAY;
	print_inf_msg("Stopping child %i\n", pid_);
	kill(pid_, SIGSTOP);

	DELAY;
	print_inf_msg("Continuing child %i\n", pid_);
	kill(pid_, SIGCONT);
/*
	DELAY;
	print_inf_msg("Ending child %i\n", pid_);
	kill(pid_, SIGTERM);
*/

	pthread_exit(pid);
}

int main(int argc, char **argv)
{
	pid_t pid, w_status;
	int status = -1;
	char *auxbin = "./tests/t_waitpid_aux";
	pthread_t th;

	switch (pid = fork()) {
	case 0:
		execl(auxbin, auxbin, argv[1], NULL);
		print_current_error_msg("%s", "ERROR: Execl");
		exit(1);
	case -1:
		print_current_error();
		break;
	default:
		do {
			pthread_create(&th, NULL, send_signals_to_child,
				       (void *) &pid);
			w_status = waitpid(pid, &status, 0);
			print_inf_msg("Waitpid end wait status[%i] "
				      "child status[%i]\n", w_status, status);

			if (w_status == -1)
				print_current_error_msg("%s\n", "ERROR: Waitpid");

			if (!NORMAL_EXIT(status)) {
				print_current_error_msg("%s\n", "Child abnormally \
							ended.");
				exit(1);
			}
			if (ERROR_EXIT(status)) {
				print_err_msg("%s\n", "Children exit error.");
				exit(1);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 0;
}
