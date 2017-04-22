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

#include "serial_start.h"
#include "screen_print.h"
#include "filesistem.h"

#define START "start"
#define STOP "stop"

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
		print_err_msg("ERROR: %s is not a start/stop script\n",
		                 script_name);
	}
}

void fork_and_exec_script(char *script_name)
{
	int pid;
	int status = -1;

	switch (pid = fork()) {
	case 0:
		exec_script(script_name);
		/* If we got here something went wrong. */
		print_current_error_msg("Error to execute script %s",
		                        script_name);
		exit(1);
		break;
	case -1:
		print_current_error_msg("Fork error to %s", script_name);
		break;
	default:
		waitpid(pid, &status, 0);
		/* TODO: check status. */
	}
}

/*
 * Run all scripts listed on script_list located in dirname directory.
 *
 * During execution script_list and its contents are released.
 */
void exec_all_scripts(char *dirname, struct dirent ***script_list, int list_len)
{
	struct dirent **list = *script_list;
	int i;

	chdir(dirname);
	for (i = 0; i < list_len; i++) {
		fork_and_exec_script(list[i]->d_name);
		free(list[i]);
	}
	free(list);
}

/*
 * Serial start of dirname init scripts.
 */
int serial_start(char act_runlevel, char prev_runlevel)
{
	struct dirent **script_list;
	char *dirname;
	int list_len;

	dirname = get_script_directory(act_runlevel);
	switch (prev_runlevel) {
	case RUNLEVEL_NONE:
		list_len = get_start_init_scripts(dirname, &script_list);
		break;
	default:
		list_len = get_change_init_scripts(act_runlevel, prev_runlevel,
		                                          &script_list);
	}

	if (list_len < 0) {
		print_current_error();
		return 1;
	}

	exec_all_scripts(dirname, &script_list, list_len);
	free(dirname);
	return 0;
}
