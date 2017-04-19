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
#include "screen_print.h"
#include "serial_start.h"

#define SET_RUNLEVEL_INFO(prev, act) act = getenv("RUNLEVEL"); \
                                     prev = getenv("PREVLEVEL")
#define IS_SYS_BOOT(prev, act) \
	prev[0] == RUNLEVEL_NONE && act[0] == RUNLEVEL_START

void print_usage(char *app_name)
{
	print_text_msg_ln(CAOS_BANNER);
	printf("Welcome to %s\n", CAOS_SHORT_DESC);
	printf("Usage: %s {0|1|2|3|4|5|6|S}\n", app_name);
	exit(1);
}


int main(int argc, char **argv)
{
	int init_errors;
	char *prev_runlevel; /* Previous runlevel */
	char *act_runlevel;  /* Actual or new runlevel */

	if (argc != 2)
		print_usage(argv[0]);

	SET_RUNLEVEL_INFO(prev_runlevel, act_runlevel);

	if (act_runlevel == NULL || prev_runlevel == NULL) {
		print_err_msg_ln("You must perform the change of runlevel"
		                 "from init, see init (8)");
		print_usage(argv[0]);
	}

	if (IS_SYS_BOOT(prev_runlevel, act_runlevel))
		PRINT_APP_INFO;

	print_inf_msg_ln("%s: swiching from runlevel[%s] to runlevel[%s]",
	                 argv[0], prev_runlevel, act_runlevel);

	init_errors = serial_start(act_runlevel[0], prev_runlevel[0]);

	if (init_errors)
		print_err_msg_ln("Error(s) detected, see log");

	return 0;
}
