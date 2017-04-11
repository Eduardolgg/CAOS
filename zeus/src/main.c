/*
 * Zeus main. It should be called by init to launch scripts of the
 * different runlevels.
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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "screen_print.h"
#include "serial_start.h"

#define SET_RUNLEVEL_INFO(prev, act) act = getenv("RUNLEVEL"); \
                                     prev = getenv("PREVLEVEL")

void print_usage(char *app_name) {
	print_text_msg_ln(CAOS_BANNER);
	printf("Welcome to %s\n", CAOS_SHORT_DESC);
	printf("Usage: %s {0|1|2|3|4|5|6|S}\n", app_name);
	exit(1);
}

/*
 * Return init scripts directory.
 * This methos use malloc to allocate the returned value, you must free it
 * in your code.
 * 
 * Returns a pointer to scripts directory path, NULL is code is unknown.
 */
char *get_script_directory(char code)
{
	char *script_dir = (char *) malloc((sizeof (char*)) * RCX_BUFFER_LEN);

	switch (code) {
	case 'S':
		PRINT_APP_INFO;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		strcpy(script_dir, RCX_DIR);
		script_dir[7] = code;
		break;
	/* Objetivo tests */
	#ifdef develop
	case 'T':
		strcpy(script_dir, "../tests/fake_init.d");
		break;
	#endif
	default:
		free(script_dir);
		script_dir = NULL;
	}

	return script_dir;
}

int main(int argc, char **argv)
{
	int init_errors;
	char *script_dir;
	char *prev_runlevel; /* Previous runlevel */
	char *act_runlevel;  /* Actual or new runlevel */

	if (argc != 2)
		print_usage(argv[0]);

	if ((script_dir = get_script_directory(argv[1][0])) == NULL)
		print_usage(argv[0]);

	SET_RUNLEVEL_INFO(prev_runlevel, act_runlevel);
	if (prev_runlevel == NULL || act_runlevel == NULL) {
		print_err_msg_ln("You must perform the change of runlevel from init, see init (8)");
		goto exit;
    }

	print_inf_msg_ln("Swiching from runlevel[%s] to runlevel[%s]", 
	    prev_runlevel, act_runlevel);
	
	init_errors = serial_start(script_dir, act_runlevel[0], prev_runlevel[0]);
	if (init_errors != 0) {
		print_err_msg_ln("Error(s) detected, see log");
	}

exit:
	free(script_dir);
	return 0;
}
