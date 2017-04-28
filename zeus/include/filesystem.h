#pragma once

/*
 * Filesistem access.
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

#include <dirent.h>

#include "runlevel_utils.h"

/*
 * Allows to manage script lists in one place.
 */
struct script_list {
	struct dirent **list;
	int len;
};

/*
 * All init scripts in rcX directories have a prefix in that format
 * [S|K][0-9]{2} for example S01apache2 or K01apache2, these is
 * the size of prefix init script.
 */
#define START_STOP_PREFIX_SIZE 3

/*
 * Related to the previous macro.
 * All start script name start with S and all stop scripts
 * start with K.
 * See info readdir and look up for "struct dirent"
 */
#define IS_START_SCRIPT(dirent) (dirent->d_name)[0] == 'S'
#define IS_STOP_SCRIPT(dirent) (dirent->d_name)[0] == 'K'
#define IS_START_STOP_SCRIPT(dirent) IS_START_SCRIPT(dirent) || \
                                     IS_STOP_SCRIPT(dirent)

/*
 * Sometimes if we compare for example ... / rc2.d with ... / rc4.d we need
 * to know if the scripts are of the same type, in other words if they start
 * with K or S.
 */
#define MATCH_SCRIPT_TYPE(a,b) a[0] == b[0]

/*
 * This macro allows you to check the name of the script without regard
 * to the prefix.
 */
#define MATCH_START_STOP_SCRIPT_NAME(dirent, char_name) \
	strcmp((dirent->d_name) + START_STOP_PREFIX_SIZE, \
	char_name + START_STOP_PREFIX_SIZE) == 0


/*
 * Alias for scandir getting only the S "start" scripts from
 * dir directory. See scandir(3).
 *
 * char *dirname: Scripts directory path.
 * struc dirent ***namelist: Empty, to alocate script list.
 */
#define get_start_init_scripts(dir, namelist) \
	scandir(dir, namelist, boot_select, alphasort)

/*
 * This function traverses the list releasing the memory of
 * each file pointed by script_list, then free the script_list memory.
 */
void free_script_list(struct script_list *script_list);
void free_script_llist(struct dirent ***script_list, int list_len);

/*
 * Calculate the scripts pending execution between the previous runlevel
 * and the current runlevel.
 *
 * This function returns the number of scripts to execute. The list of
 * scripts is stored in script_list. It is the responsibility of the
 * calling function to free the memory assigned to script_list.
 *
 * Note: The operation is similar to scandir, it actually calls scandir
 * several times.
 */
int get_change_init_scripts(struct runlevel *prev_level,
                            struct runlevel *new_level,
                            struct dirent ***script_list);

/*
 * Allows you to select only the startup scripts, starting with S.
 *
 * Return 1 if is a start script, 0 otherwise.
 */
int boot_select(const struct dirent *var);

/*
 * Allows you to select start/stop scripts, starting with S or K.
 *
 * Return 1 if is a start/stop script, 0 otherwise.
 */
int change_runlevel_select(const struct dirent *var);

/*
 * Return init scripts directory.
 * This methos use malloc to allocate the returned value, you must free it
 * in your code.
 *
 * Returns a pointer to scripts directory path, NULL is code is unknown.
 */
char *get_script_directory(char code);
