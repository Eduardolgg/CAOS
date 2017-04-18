/*
 * Run init scripts in serial mode.
 *
 *		11-Apr-2017 Elgg
 *
 *		This file is part of the CAOS init suite,
 *		Copyright (C) 2017 Eduardo L. García Glez.
 *
 *		This program is free software; you can redistribute it and/or modify
 *		it under the terms of the GNU General Public License as published by
 *		the Free Software Foundation; either version 2 of the License, or
 *		(at your option) any later version.
 *
 *		This program is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with this program; if not, you can get a copy at
 *		<http://www.gnu.org/licenses/>
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

#define FIRST_CHAR(script_name) script_name[0]
#define START "start"
#define STOP "stop"

void exec_script(char *script_name)
{
	switch(FIRST_CHAR(script_name)) {
	case 'S':
		execl(script_name, script_name, START, NULL);
		break;
	case 'K':
		execl(script_name, script_name, STOP, NULL);
		break;
	default:
		print_err_msg_ln("ERROR: %s is not a start/stop script", 
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
		print_current_error_msg("Error to execute script %s", script_name);
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
 * Serial start of dirname init scripts.
 */
int serial_start(char *act_dirname, char act_runlevel, 
	char *prev_dirname, char prev_runlevel)
{
	struct dirent **start_script_list;
	int n, i;

	switch(prev_runlevel) {
	case 'N': 
		n = get_start_init_scripts(dirname, &start_script_list);
		break;
	default:
		n = get_change_init_scripts(dirname, &start_script_list);
		
	}

	if (n < 0) {
		print_current_error();
		return 1;
	}

	chdir(dirname);
	for (i = 0; i < n; i++) {
		fork_and_exec_script(start_script_list[i]->d_name);
		free(start_script_list[n]);
	}
	free(start_script_list);
	return 0;
}
