/*
 * Run init scripts in serial mode.
 *
 *    11-Apr-2017 Elgg
 *
 *    This file is part of the CAOS init suite,
 *    Copyright (C) 2017 Eduardo L. García Glez.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, you can get a copy at
 *    <http://www.gnu.org/licenses/>
 */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

#include "log.h"
#include "filesystem.h"
#include "processes.h"
#include "runlevel_utils.h"

#define START "start"
#define STOP "stop"

static
void exec_script(char *script_name)
{
	switch (script_name[0]) {
	case 'S':
		execl(script_name, script_name, START, NULL);
		break;
	case 'K':
		execl(script_name, script_name, STOP, NULL);
		break;
	default:
		print_err_msg("ERROR: %s is not a start/stop script, it will "
		              "not run.\n", script_name);
	}
}

static
void wait_for_child(pid_t pid, char *script_name)
{
	int w_status,
	    status = -1;

	w_status = waitpid(pid, &status, 0);

	if (w_status == -1) {
		print_current_error_msg("%s: ", script_name);
		return;
	}

	if (NORMAL_EXIT(status))
		syslog_info("%s ran correctly.", script_name);
	else
		print_current_error_msg("%s: abnormally ended.",
					script_name);

	if (NORMAL_EXIT(status) && ERROR_EXIT(status))
		print_err_msg("%s: exit code [%i]", script_name,
			      WEXITSTATUS(status));
}

static
void *fork_and_exec_script(void *script_name)
{
	pid_t pid;

	switch (pid = fork()) {
	case 0:
		exec_script((char *) script_name);
		/* If we got here something went wrong. */
		print_current_error_msg("Error to execute script %s",
		                        (char *) script_name);
		exit(1);
		break;
	case -1:
		print_current_error_msg("Fork error to %s", (char *) script_name);
		break;
	default:
		wait_for_child(pid, (char *) script_name);
	}
	pthread_exit(script_name);
}

#define wait_for_thread(pth_list, pth_aux, pth_output, status)           \
({                                                                       \
	int pth_error = pthread_join(*--pth_aux, (void **) &pth_output); \
	if (pth_error != 0) {                                            \
		print_current_error();                                   \
		status = -1;                                             \
	}                                                                \
})


#define wait_for_all_threads(pth_list, pth_aux, pth_output, status)          \
({                                                                           \
	while (pth_aux != pth_list) {                                        \
		wait_for_thread(pth_list, pth_aux, pth_output, status);      \
	}                                                                    \
})

#define list_end(index, len) (index == list_len - 1)

#define same_level(script_a, script_b)        \
	(strncmp(script_a, script_b, 3) == 0)

#define MAX_THREADS 4
#define time_to_wait(script_list, len, index)                                 \
	(list_end(index, len) ||                                              \
	(!list_end(index, len) && !same_level(script_list[index + 1]->d_name, \
	                                      script_list[index]->d_name)))
/*
 * Run all scripts listed on script_list located in dirname directory.
 *
 * Return 0 on success, -1 on fail.
 */
static
int exec_all_scripts(char *dirname, struct dirent ***script_list, int list_len)
{
	char *pth_output;
	struct dirent **list = *script_list;
	int i, error, status = 0;
	pthread_t *pth_list, *pth_aux;

	pth_list = (pthread_t *) malloc(sizeof(pthread_t) * list_len);
	pth_aux = pth_list;

	if (!pth_list || chdir(dirname)) {
		print_current_error();
		status = -1;
		goto finalize;
	}

	for (i = 0; i < list_len; i++) {
		error = pthread_create(pth_aux++, NULL,
		                       fork_and_exec_script,
		                       list[i]->d_name);
		if (error != 0) {
			print_current_error();
			status = -1;
		}

		if (time_to_wait(list, list_len, i))
			wait_for_all_threads(pth_list, pth_aux, pth_output, status);
		else if ((pth_aux - pth_list) >= MAX_THREADS)
			wait_for_thread(pth_list, pth_aux, pth_output, status);
	}
finalize:
	free(pth_list);
	return status;
}

/*
 * Change from prev_level runlevel to new_level runlevel in parallel
 * mode, is based on the order given in the startup scripts.
 *
 * If prev_level code is equal to RUNLEVEL_NONE, all scripts in
 * new_runlevel are runned.
 *
 * The returned value is equal to zero if no error was detected, On
 * error return 1.
 *
 * Note: This function send status information to the screen
 * and to syslog.
 */
int low_parallel_start(struct runlevel *prev_level, struct runlevel *new_level)
{
	struct dirent **script_list;
	int list_len, exec_output;

	if (IS_SYS_BOOT(prev_level->code))
		list_len = get_start_init_scripts(new_level->dir,
		                                  &script_list);
	else
		list_len = get_change_init_scripts(prev_level, new_level,
		                                   &script_list);
	if (list_len < 0) {
		print_current_error();
		return 1;
	}

	exec_output = exec_all_scripts(new_level->dir, &script_list, list_len);
	free_script_llist(&script_list, list_len);
	return exec_output;
}
