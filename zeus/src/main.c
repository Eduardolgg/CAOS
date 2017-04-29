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
#include <stdlib.h>

#include "config.h"
#include "log.h"
#include "runlevel_utils.h"
#include "serial_start.h"

char *app_path;

void print_usage()
{
	print_text_msg("%s", CAOS_BANNER);
	printf("Welcome to %s\n", CAOS_SHORT_DESC);
	printf("Usage: %s {0|1|2|3|4|5|6|S}\n", app_path);
}

/*
 * Execution ends if runlevels are invalid.
 */
void exit_invalid_runlevels(char prev_runlevel, char new_runlevel)
{
	print_usage();
	print_err_msg("You must perform the change of runlevel "
		      "from init, see init (8)\n"
		      "Invalid runlevels: prev[%c], act[%c]\n",
		       prev_runlevel, new_runlevel);
	exit(1);
}

int main(int argc, char **argv)
{
	int init_errors;
	struct runlevel prev_runlevel; /* Previous runlevel */
	struct runlevel new_runlevel;  /* Actual or new runlevel */

	app_path = argv[0];
	if (argc != 2) {
		print_usage();
		exit(1);
	}

	if (set_runlevel_info(&prev_runlevel, &new_runlevel))
		exit_invalid_runlevels(prev_runlevel.code, new_runlevel.code);

	if (IS_SYS_BOOT_START(prev_runlevel.code, new_runlevel.code))
		PRINT_APP_INFO;

	print_inf_msg("%s: swiching from runlevel[%c] to runlevel[%c]\n",
	              APP_NAME, prev_runlevel.code, new_runlevel.code);

	init_errors = serial_start(&prev_runlevel, &new_runlevel);

	if (init_errors)
		print_err_msg("Error(s) detected, see log\n");

	free_runlevel_items(&prev_runlevel);
	free_runlevel_items(&new_runlevel);

	return 0;
}
