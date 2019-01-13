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
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <string.h>

#include <pty.h>
#include <utmp.h>

#include "log.h"
#include "filesystem.h"
#include "processes.h"
#include "runlevel_utils.h"

#define START "start"
#define STOP "stop"

static
int is_virtual_terminal_available() {
	struct stat s;

	return stat("/dev/pts", &s) != -1 && stat("/dev/ptmx", &s) != -1;
}

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
		syslog_info("LP: %s ran correctly.", script_name);
	else
		print_current_error_msg("%s: abnormally ended.\n",
					script_name);

	if (NORMAL_EXIT(status) && ERROR_EXIT(status))
		print_err_msg("%s: exit code [%i]\n", script_name,
			      WEXITSTATUS(status));
}

// TODO: try to use for interactive start scripts
static
void send_pty_io_to_standar_io(struct proc_info *thr_data)
{
	struct stat stat_info;

	if (stat("/dev/pts", &stat_info) != -1) {
		if (dup2(thr_data->fd_slave, STDIN_FILENO) != STDIN_FILENO)
			print_current_error_msg("Error dup slave fd to stdin\n");
		if (dup2(thr_data->fd, STDOUT_FILENO) != STDOUT_FILENO)
			print_current_error_msg("Error dup slave fd to stdout\n");
		if (dup2(thr_data->fd, STDERR_FILENO) != STDERR_FILENO)
			print_current_error_msg("Error dup slave fd to stderr\n");

		close(thr_data->fd);
		close(thr_data->fd_slave);
	}
}

static
void *fork_and_exec_script(void *thread_data)
{
	pid_t pid;
	struct proc_info *thr_data;

	thr_data = (struct proc_info *) thread_data;

	if (thr_data->config->lsb.user_interactive)
		pid = fork();
	else if (is_virtual_terminal_available())
		pid = forkpty(&(thr_data->fd), NULL, NULL, NULL);
	else {
		print_current_error_msg("Console unavailable %s\n",
		                                         thr_data->script_name);
		pid = fork();
	}

	switch (pid) {
	case 0:
		(void)signal(SIGINT, SIG_IGN);
		(void)signal(SIGQUIT, SIG_IGN);
		(void)signal(SIGTERM, SIG_IGN);
		(void)signal(SIGTSTP, SIG_IGN);
		(void)signal(SIGCHLD, SIG_DFL);

		exec_script(thr_data->script_name);
		// If we got here something went wrong.
		print_current_error_msg("Error to execute script %s\n",
		                        thr_data->script_name);
		exit(1);
		break;
	case -1:
		print_current_error_msg("Fork error to %s\n",
		                        thr_data->script_name);
		break;
	default:
		wait_for_child(pid, thr_data->script_name);
		fsync(thr_data->fd_slave);
	}
	pthread_exit(thr_data->script_name);
}

void wait_for_thread(struct proc_info **process,
			    int status)
{
	struct proc_info *pth_aux = *process;
	int pth_error = pthread_join((--pth_aux)->thread, NULL);
	if (pth_error != 0) {
		print_current_error();
		status = -1;
	}  else {
		int wft_rb;
		char wft_buf[256];
		do {
			memset(wft_buf, '\0', sizeof(wft_buf));
			wft_rb = read(pth_aux->fd, wft_buf, sizeof(wft_buf)-1);
			printf("%s", wft_buf);
		} while (wft_rb > 0);
		if (pth_aux->config != NULL)
			free_script_config(pth_aux->config);
		close(pth_aux->fd);
	}
	*process = pth_aux;
}


void wait_for_all_threads(struct proc_info *pth_list,
				 struct proc_info **pth_aux,
				 int status)
{
	while (*pth_aux != pth_list) {
		wait_for_thread(pth_aux, status);
	}
}

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
	struct dirent **list = *script_list;
	int i, error, status = 0;
	struct proc_info *p_list, *p_aux;
	struct script_config* config = NULL;

	p_list = (struct proc_info *) malloc(sizeof(struct proc_info)
	                                                            * list_len);
	p_aux = p_list;

	if (!p_list || chdir(dirname)) {
		print_current_error();
		status = -1;
		goto finalize;
	}

	for (i = 0; i < list_len; i++) {
		config = (struct script_config*) malloc(sizeof(
		                                         struct script_config));
		config->script_name = list[i]->d_name;
		read_script_config(config);

		p_aux->script_name = list[i]->d_name;
		p_aux->config = config;

		error = pthread_create(&(p_aux->thread), NULL,
		                       fork_and_exec_script, p_aux);
		p_aux++;
		if (error != 0) {
			print_current_error();
			status = -1;
		}

		if (time_to_wait(list, list_len, i))
			wait_for_all_threads(p_list, &p_aux, status);
		else if ((p_aux - p_list) >= MAX_THREADS || config->lsb.user_interactive)
			wait_for_thread(&p_aux, status);
#ifdef DEBUG
		struct proc_info *aa;
		aa = p_aux;
		while (aa-- != p_list)
			print_dbg_msg("whaiting for %s\n", aa->script_name);
#endif

	}
finalize:
	free(p_list);
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
