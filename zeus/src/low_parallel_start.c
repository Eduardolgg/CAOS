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
int is_virtual_terminal_available()
{
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
		print_current_error_msg("%s: \n", script_name);
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

// TODO: try to use pty for interactive start scripts like a ssh connection.
/*static
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
}*/

void fork_and_exec_script(int is_interactive, int *fd, char *script_name)
{
	pid_t pid;

	if (is_interactive) {
		print_dbg_msg("Forking interactive script %s\n", script_name);
		pid = fork();
	} else if (is_virtual_terminal_available()) {
		print_dbg_msg("Forking NO interactive script on pseudo-tty %s\n",
		               script_name);
		pid = forkpty(fd, NULL, NULL, NULL);
	} else {
		print_current_error_msg("Pseudo-tty unavailable for interactive \
		                         script %s\n", script_name);
		pid = fork();
	}

	switch (pid) {
	case 0:
		(void)signal(SIGINT, SIG_IGN);
		(void)signal(SIGQUIT, SIG_IGN);
		(void)signal(SIGTERM, SIG_IGN);
		(void)signal(SIGTSTP, SIG_IGN);
		(void)signal(SIGCHLD, SIG_DFL);

		exec_script(script_name);
		// If we got here something went wrong.
		print_current_error_msg("Error to execute script %s\n",
		                                                  script_name);
		exit(1);
		break;
	case -1:
		print_current_error_msg("Fork error to %s\n", script_name);
		break;
	default:
		wait_for_child(pid, script_name);
	}
}

void *fork_and_exec_script_in_thread(void *thread_data)
{
	struct proc_info *thr_data;

	thr_data = (struct proc_info *) thread_data;
	print_dbg_msg("Forking in thread: %s\n", thr_data->script_name);
	fork_and_exec_script(thr_data->is_interactive, &(thr_data->fd),
			     thr_data->script_name);
	thr_data->is_thread_end = 1;
	print_dbg_msg("Thread end for: %s\n", thr_data->script_name);
	pthread_exit(thr_data->script_name);
}

void pty_to_stdout(int fd)
{
	int readed;
	char buffer[256];

	if (fd < 0)
		return;
	do {
		memset(buffer, '\0', sizeof(buffer));
		readed = read(fd, buffer, sizeof(buffer)-1);
		printf("%s", buffer);
	} while (readed > 0);
}

void remove_queue_item(struct proc_info **process,
		       struct proc_info **head)
{
	struct proc_info *item_to_remove, *prev_item, *next_item;

	prev_item = (*process)->prev;
	next_item = (*process)->next;
	item_to_remove = *process;

	if (*head == (*head)->next) {
		*head = NULL;
		next_item = NULL;
		goto free;
	} else if (*head == item_to_remove)
		*head = next_item;

	if (prev_item)
		prev_item->next = item_to_remove->next;
	if (next_item)
		next_item->prev = item_to_remove->prev;
free:
	free_proc_info(process);
	*process = next_item;
}

int wait_for_thread(struct proc_info **process, struct proc_info **head)
{
	int status = -1;

	print_dbg_msg("Waiting for thread %s\n", (*process)->script_name);
	if (!(*process)->is_interactive) {
		int pth_error = pthread_join((*process)->thread, NULL);
		if (pth_error != 0)
			print_current_error_msg("%s\n", "Thread error:");
	}
	pty_to_stdout((*process)->fd);
	remove_queue_item(process, head);
	status = 0;
	return status;
}

int wait_for_all_threads(struct proc_info **pth_list)
{
	int status = 0;
	struct proc_info *head = (*pth_list);

	while (*pth_list)
		// TODO: wait if pth_list == head.
		if ((*pth_list)->is_thread_end)
			status += wait_for_thread(pth_list, &head);
		else
			*pth_list = (*pth_list)->next;
	pth_list = NULL;

	return status;
}

#define list_end(index, len) (index == list_len - 1)

#define same_level(script_a, script_b)        \
	(strncmp(script_a, script_b, 3) == 0)

#define time_to_wait(script_list, len, index)                                 \
	(list_end(index, len) ||                                              \
	(!list_end(index, len) && !same_level(script_list[index + 1]->d_name, \
	                                      script_list[index]->d_name)))

/*
 * Add item to p_list.
 *
 * Return Pointer to added item, NULL otherwise. P_list points to new item if
 * p_list is null.
 */
struct proc_info* add_proc_item(struct proc_info **p_list,
				char* script_name)
{
	struct proc_info *p_info = NULL,
			 *end_item;

	p_info = malloc_proc_info(script_name);

	if (p_info) {
		if (!*p_list) {
			*p_list = p_info;
		} else {
			end_item = (*p_list)->prev;
			(*p_list)->prev = p_info;
			end_item->next = p_info;

			p_info->prev = end_item;
			p_info->next = (*p_list);
		}
	} else
		print_current_error();
	return p_info;
}

/*
 * Run all scripts listed on script_list located in dirname directory.
 *
 * Return 0 on success, -1 on fail.
 */
static
int exec_all_scripts(char *dirname, struct dirent ***script_list, int list_len)
{
	struct dirent **list = *script_list;
	int i, status = 0, active_threads = 0;
	struct proc_info *p_list = NULL,
	       *last_proc = NULL;

	if (chdir(dirname)) {
		print_current_error();
		goto finalize;
	}

	for (i = 0; i < list_len; i++) {
		print_dbg_msg("In script %s: \n", list[i]->d_name);
		if (!(last_proc = add_proc_item(&p_list, list[i]->d_name)) ||
                      pthread_create(&(last_proc->thread), NULL,
		                   fork_and_exec_script_in_thread, last_proc)) {
			int fd;
			print_current_error();
			fork_and_exec_script(0, &fd, list[i]->d_name);

		} else
			active_threads++;

		// Delete status info before print prosess output.
		/*char msgStatus[81];
		memset(msgStatus, '\b', sizeof(msgStatus - 1));
		msgStatus[80] = '\0';
		print("%s", msgStatus);*/

		if (time_to_wait(list, list_len, i))
			status += wait_for_all_threads(&p_list);
		else if (active_threads >= MAX_THREADS || last_proc->is_interactive)
			status += wait_for_thread(&last_proc, &p_list);
/* #ifdef DEBUG
		if (p_list) {
			struct proc_info *aa = p_list;
			do {
				print_dbg_msg("Whaiting for %s\n", aa->script_name);
				aa = aa->next;
			} while (aa != p_list);
		}
#endif*/

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

	print_dbg_msg("Selecting boot type %c->%c\n", prev_level->code,
	                                                       new_level->code);
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
