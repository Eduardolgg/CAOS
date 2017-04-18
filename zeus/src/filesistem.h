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

#include <dirent.h>

#define START_STOP_PREFIX_SIZE 3
#define IS_START_SCRIPT(dirent) (dirent->d_name)[0] == 'S'
#define IS_STOP_SCRIPT(dirent) (dirent->d_name)[0] == 'K'
#define IS_START_STOP_SCRIPT(dirent) IS_START_SCRIPT(dirent) || \
                                     IS_STOP_SCRIPT(dirent)
#define MATCH_SCRIPT_TYPE(a,b) a[0] == b[0]
#define MATCH_START_STOP_SCRIPT_NAME(dirent, char_name) strcmp((dirent->d_name) + START_STOP_PREFIX_SIZE, char_name) == 0


/*
 * Alias for scandir getting only the S "start" scripts from
 * dir directory. See scandir(3).
 * 
 * char *dirname: Scripts directory path.
 * struc dirent ***namelist: Empty, to alocate script list.
 */
#define get_start_init_scripts(dir, namelist) scandir(dir, namelist, boot_select, alphasort)

/*
 * Alias for scandir getting S and K (start/stop) scripts from
 * dir directory. See scandir(3).
 * 
 * char *dirname: Scripts directory path.
 * struc dirent ***namelist: Empty, to alocate script list.
 */
#define get_change_init_scripts(dir, namelist) scandir(dir, namelist, change_runlevel_select, alphasort)

/*
 * Allows you to get only the startup scripts, starting with S.
 * 
 * Return 1 if is a start script, 0 otherwise.
 */
int boot_select(const struct dirent *var);

/*
 * Allows you to get start/stop scripts, starting with S or K.
 * 
 * Return 1 if is a start/stop script, 0 otherwise.
 */
int change_runlevel_select(const struct dirent *var);
