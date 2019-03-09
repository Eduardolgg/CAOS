/*
 * Filesystem access.
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

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"
#include "log.h"
#include "filesystem.h"
#include "runlevel_utils.h"

/*
 * Return init scripts directory.
 * This methos use malloc to allocate the returned value, you must free it
 * in your code.
 *
 * Returns a pointer to scripts directory path, NULL if code is unknown.
 */
char *get_script_directory(char code)
{
	char *script_dir;

	if (!IS_VALID_LEVEL(code))
		return NULL;

	script_dir = (char *) malloc((sizeof (char*)) * RCX_BUFFER_LEN);
	strcpy(script_dir, RCX_DIR);
	script_dir[RCX_XCHAR_LOCATION] = code;
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
static
int list_contains_script(struct script_list *script_list, char *script)
{
	int i;
	struct dirent **list = script_list->list;

	for (i = 0; i < script_list->len; i++) {
		if(MATCH_START_STOP_SCRIPT_NAME(list[i], script) &&
		   MATCH_SCRIPT_TYPE(list[i]->d_name, script))
			return 1;
	}
	return 0;
}

/*
 * This function traverses the list releasing the memory of
 * each file pointed by script_list, then free the script_list memory.
 */
void free_script_list(struct script_list *script_list)
{
	int i;

	for (i = 0; i < script_list->len; i++) {
		if (script_list->list[i] != NULL)
			free(script_list->list[i]);
	}

	free(script_list->list);
	script_list->list = NULL;
	script_list->len = -1;
}

/*
 * See free_script_list function.
 */
void free_script_llist(struct dirent ***script_list, int list_len)
{
	struct script_list slist;

	slist.list = *script_list;
	slist.len = list_len;

	free_script_list(&slist);
	*script_list = NULL;
}

int script_diff(struct script_list *prev,
                struct script_list *new,
                struct script_list *dest)
{
	int i;

	dest->len = 0;
	for (i = 0; i < new->len; i++) {
		if (!list_contains_script(prev, (new->list)[i]->d_name)) {
			// Mover el elemento de una lista a otra.
			(dest->list)[dest->len++] = (new->list)[i];
			(new->list)[i] = NULL;
		}
	}
	return dest->len;
}

#define init_script_list(list_, runlevel)                        \
	(list_.len = scandir(runlevel->dir, &(list_.list),        \
	                    change_runlevel_select, alphasort))

#define set_list_mem(list, len) \
	(list = malloc(sizeof(struct dirent *) * len))

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
                            struct dirent ***script_list)
{
	struct script_list prev_slist = { .list = NULL, .len = -1 },
		           new_slist = { .list = NULL, .len = -1 },
		           final_slist = { .list = NULL, .len = -1 };

	*script_list = NULL;

	if (init_script_list(prev_slist, prev_level) < 0)
		goto exit;

	if (init_script_list(new_slist, new_level) < 0)
		goto exit;

	if (!set_list_mem(final_slist.list, new_slist.len))
		goto exit;

	script_diff(&prev_slist, &new_slist, &final_slist);
exit:
	free_script_list(&new_slist);
	free_script_list(&prev_slist);

	*script_list = final_slist.list;
	return final_slist.len;
}

#define is_buffer_enough(buffer_size, script_name_size) \
	(buffer_size > script_name_size + sizeof(CAOS_CONFIG_DIR_USER_INTERACTIVE))

int is_user_interactive(char* script_name)
{
	struct stat s;
	int buffer_len = 256;
	char script_path[buffer_len];

	if (!is_buffer_enough(sizeof(script_path), sizeof(script_name))) {
		print_war_msg("Path to %s is too large, I treat it as "
		              "interactive script", script_name);
		return 1;
	}

	memset(script_path, '\0', sizeof(script_path));
	strncpy(script_path, CAOS_CONFIG_DIR_USER_INTERACTIVE,
	                              sizeof(CAOS_CONFIG_DIR_USER_INTERACTIVE));
	strcat(script_path, script_name);
	print_inf_msg("%s\n", script_path);
	return stat(script_path, &s) != -1;
}
