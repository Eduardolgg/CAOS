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

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "config.h"
#include "log.h"
#include "low_parallel_start.h"
#include "run.h"
#include "runlevel_utils.h"
#include "serial_start.h"

#define DEFAULT_STARTUP_ALGORITHM "default"

char *app_path;
/*
 * Store algorithm name and startup function pointer.
 */
struct startup_alg {
	char *name;
	int (*function)(struct runlevel*, struct runlevel*);
};

/*
 * List of valid startup algorithms and functions.
 *
 * If you want to add a new algorithm add new name and function
 * pointer to these list.
 */
struct startup_alg startup_mode[] = {
	{.name = "low_parallel", .function = &low_parallel_start},
	{.name = "serial", .function = &serial_start},
	{.name = DEFAULT_STARTUP_ALGORITHM, .function = &serial_start}
};

void print_usage()
{
	print_text_msg("%s", CAOS_BANNER);
	printf("Welcome to %s\n", CAOS_INFO);
	printf("Usage: %s {0|1|2|3|4|5|6|S}\n", app_path);
}

static inline void print_normal_end_msg_if_debug(struct runlevel* prev_runlevel,
					  struct runlevel* new_runlevel) {
#ifdef DEBUG
	if (!IS_SYS_BOOT_START(prev_runlevel->code, new_runlevel->code)) {
		print_dbg_msg("ZEUS: The work is done, I must rest.\n");
	}
#endif
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

#define MATCH_ALGORITHM(alg, item) strcmp(alg, item->name) == 0

/*
 * We are at the end of the list if item points to DEFAULT_STARTUP_ALGORITHM
 */
#define NOT_LIST_END(item) strcmp(item->name, DEFAULT_STARTUP_ALGORITHM) != 0

/*
 * Find st_algorithm in startup_mode list.
 *
 * If st_algorithm is found in startup_mode list, it is stored in st_item
 * with startup function and return zero. If st_algorithm is not found
 * st_item is set with DEFAULT_STARTUP_ALGORITHM and -1 is returned.
 */
int get_startup_function(char *st_algorithm, struct startup_alg *st_item)
{
	int status = -1;
	struct startup_alg *item = startup_mode;
	do {
		if (MATCH_ALGORITHM(st_algorithm, item)) {
			status = 0;
			break;
		}
		item++;
	} while (NOT_LIST_END(item));

	*st_item = *item;
	return status;
}

int main(int argc, char **argv)
{
	int init_errors = 0;
	struct startup_alg st_mode;    /* Startup mode item */
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

	make_killall_ignore_me();

	if (get_startup_function(argv[1], &st_mode) != 0) {
		init_errors = 1;
		print_war_msg("%s is NOT a valid startup mode. Using %s\n",
			      argv[1], st_mode.name);
	}

	init_errors |= st_mode.function(&prev_runlevel, &new_runlevel);

	if (init_errors)
		print_err_msg("Error(s) detected, see log\n");

	free_runlevel_items(&prev_runlevel);
	free_runlevel_items(&new_runlevel);
	remove_killall_ignore();

	print_normal_end_msg_if_debug(&prev_runlevel, &new_runlevel);

	return init_errors;
}
