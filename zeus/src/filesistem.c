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

#include <string.h>
#include "filesistem.h"

/*
 * Allows you to get only the startup scripts, starting with S.
 * 
 * Return 1 if is a start script, 0 otherwise.
 */
int boot_select(const struct dirent *var)
{
	return IS_START_SCRIPT(var);
}

/*
 * Allows you to get start/stop scripts, starting with S or K.
 * 
 * Return 1 if is a start/stop script, 0 otherwise.
 */
int change_runlevel_select(const struct dirent *var)
{
	return IS_START_STOP_SCRIPT(var);
}

int list_contains_script(struct dirent **script_list, int list_len, char *script)
{
	int i;
	for (i = 0; i < n; i++) {
		
		if(MATCH_START_STOP_SCRIPT_NAME(script_list[i], script)) {
			return 1;
	}
	return 0;
}	

