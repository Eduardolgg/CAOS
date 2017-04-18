/*
 * Filesistem access.
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

#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "screen_print.h"
#include "filesistem.h"

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
		/*PRINT_APP_INFO;*/
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

/*
 * Allows you to select only the startup scripts, starting with S.
 * 
 * Return 1 if is a start script, 0 otherwise.
 */
int boot_select(const struct dirent *var)
{
	return IS_START_SCRIPT(var);
}

/*
 * Allows you to select start/stop scripts, starting with S or K.
 * 
 * Return 1 if is a start/stop script, 0 otherwise.
 */
int change_runlevel_select(const struct dirent *var)
{
	return IS_START_STOP_SCRIPT(var);
}

/*
 * Check if script_list contains the given script.
 *
 * Return 1 if script_list contains the script, 0 otherwise.
 */
int list_contains_script(struct dirent **script_list, int list_len, char *script)
{
	int i;
	for (i = 0; i < list_len; i++) {
		if(MATCH_START_STOP_SCRIPT_NAME(script_list[i], script) &&
		   MATCH_SCRIPT_TYPE(script_list[i]->d_name, script))
			return 1;
	}
	return 0;
}	

/*
 * This function traverses the list releasing the memory of
 * each file pointed by file_list, then free the file_list memory.
 */
void free_list(struct dirent ***file_list, int list_len)
{
	int i;
	struct dirent **list = *file_list;
	for (i = 0; i < list_len; i++) {
		if (list[i] != NULL)
			free(list[i]);
	}
	free(list);
}

/*
 * Alias for scandir getting S and K (start/stop) scripts from
 * dir directory. See scandir(3).
 * 
 * char *dirname: Scripts directory path.
 * struc dirent ***namelist: Empty, to alocate script list.
 */
int get_change_init_scripts(char act_runlevel, char prev_runlevel, struct dirent ***script_list)
{
	int act_script_num, prev_script_num, final_num = 0, i;
	char *act_script_dir, *prev_script_dir;
	struct dirent **act_script_list, **prev_script_list, **final_list;

	act_script_dir = get_script_directory(act_runlevel);
	prev_script_dir = get_script_directory(prev_runlevel);

	act_script_num = scandir(act_script_dir, &act_script_list, 
	                            change_runlevel_select, alphasort);
	prev_script_num = scandir(prev_script_dir, &prev_script_list, 
	                            change_runlevel_select, alphasort);

	if (act_script_num < 0 || prev_script_num < 0) {
		print_current_error();
		free(act_script_dir);
		free(prev_script_dir);
		free_list(&act_script_list, act_script_num);
		free_list(&prev_script_list, prev_script_num);
		return -1;
	}

	final_list = malloc(sizeof(struct dirent *) * act_script_num);

	for (i = 0; i < act_script_num; i++) {
		if (list_contains_script(prev_script_list, prev_script_num, 
		    act_script_list[i]->d_name)) {
			free(act_script_list[i]);
		} else {
			final_list[final_num++] = act_script_list[i];
		}
	}
	
	free(act_script_dir);
	free(prev_script_dir);
	free_list(&prev_script_list, prev_script_num);
	free(act_script_list);

	*script_list = final_list;
	return final_num;	
}
