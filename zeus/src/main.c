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
#include "low_parallel_start.h"

char *app_path;

struct startup_item {
	char *name;
	int (*function)(struct runlevel*, struct runlevel*);
};

#define DEFAULT_STARTUP_ALGORITHM "default"
struct startup_item startup_mode[] = {
	{.name = "low_parallel", .function = &low_parallel_start},
	{.name = "serial", .function = &serial_start},
	{.name = DEFAULT_STARTUP_ALGORITHM, .function = &serial_start}
};

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

int (*get_startup_function(char *st_algorithm)) (struct runlevel*, struct runlevel*)
{
	struct startup_item *sm = startup_mode;
	do {
		if (strcmp(st_algorithm, sm->name) == 0)
			break;
		sm++;
	} while (strcmp(sm->name, DEFAULT_STARTUP_ALGORITHM) != 0);
	return sm->function;
}

int get_st_function(char *st_algorithm, struct startup_item *st_item)
{
	struct startup_item *item = startup_mode;
	do {
		if (strcmp(st_algorithm, item->name) == 0) {
			*st_item = *item;
			return 0;
		}
		item++;
	} while (strcmp(item->name, DEFAULT_STARTUP_ALGORITHM) != 0);
	return -1;
}

int main(int argc, char **argv)
{
	int init_errors;
	struct startup_item item;
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

	print_inf_msg("%s: Swiching from runlevel[%c] to runlevel[%c] - %s\n",
	              APP_NAME, prev_runlevel.code, new_runlevel.code, argv[1]);

	if (get_st_function(argv[1], &item) != 0) {
		init_errors = 1;
		print_war_msg("%s is NOT a valid startup mode.", argv[1]);
	}

	init_errors |= item.function(&prev_runlevel, &new_runlevel);

	if (init_errors)
		print_err_msg("Error(s) detected, see log\n");

	free_runlevel_items(&prev_runlevel);
	free_runlevel_items(&new_runlevel);

	return init_errors;
}
