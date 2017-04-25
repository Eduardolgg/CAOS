/*
 * Zeus main. It should be called by init to launch scripts of the
 * different runlevels.
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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "log.h"
#include "serial_start.h"

#define SET_RUNLEVEL_INFO(prev, act) act = getenv("RUNLEVEL"); \
                                     prev = getenv("PREVLEVEL")
#define IS_SYS_BOOT(prev, act) \
	prev[0] == RUNLEVEL_NONE && act[0] == RUNLEVEL_START

char *app_path;

void print_usage()
{
	print_text_msg("%s", CAOS_BANNER);
	printf("Welcome to %s\n", CAOS_SHORT_DESC);
	printf("Usage: %s {0|1|2|3|4|5|6|S}\n", app_path);
}

/*
 * Check if prev_runlevel and act_runlevel are valid runlevels.
 *
 * Returns an integer equal to zero if runlevels are valid, or
 * an integer equal to one otherwise.
 */
int check_runlevels(char *prev_runlevel, char *act_runlevel)
{
	/* NULL is not a good runlevel */
	if (prev_runlevel == NULL || act_runlevel == NULL)
		return 1;
	/* Runlevel code is only one character */
	if (!(IS_VALID_PREV_LEVEL(prev_runlevel[0]) &&
	      IS_VALID_LEVEL(act_runlevel[0])))
		return 1;

	return 0;
}

/*
 * Execution ends if runlevels are invalid.
 */
void exit_if_invalid_runlevels(char *prev_runlevel, char *act_runlevel)
{
	if (check_runlevels(prev_runlevel, act_runlevel)) {
		print_usage();
		print_err_msg("You must perform the change of runlevel "
		              "from init, see init (8)"
		              "Invalid runlevels: prev[%s], act[%s]\n",
		               prev_runlevel, act_runlevel);
		exit(1);
	}
}


int main(int argc, char **argv)
{
	int init_errors;
	char *prev_runlevel; /* Previous runlevel */
	char *act_runlevel;  /* Actual or new runlevel */

	app_path = argv[0];
	if (argc != 2) {
		print_usage();
		exit(1);
	}

	SET_RUNLEVEL_INFO(prev_runlevel, act_runlevel);
	exit_if_invalid_runlevels(prev_runlevel, act_runlevel);

	if (IS_SYS_BOOT(prev_runlevel, act_runlevel))
		PRINT_APP_INFO;

	print_inf_msg("%s: swiching from runlevel[%s] to runlevel[%s]\n",
	              APP_NAME, prev_runlevel, act_runlevel);

	init_errors = serial_start(act_runlevel[0], prev_runlevel[0]);

	if (init_errors)
		print_err_msg("Error(s) detected, see log\n");

	return 0;
}
