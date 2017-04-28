/*
 * General config.
 *
 *    26-Apr-2017 Elgg
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

#include "runlevel_utils.h"
#include "filesystem.h"

/*
 * Store runlevel information in prevlevel and newlevel
 * parameters. Runlevel information is obtained from enviroments
 * variables PREVLEVEL and RUNLEVEL see init(8) for details.
 *
 * Return 0 if runlevels can be set. On error return 1.
 */
int set_runlevel_info(struct runlevel *prevlevel, struct runlevel *newlevel)
{
	return set_prevlevel_info(prevlevel) ||
	       set_newlevel_info(newlevel);
}

/*
 * See set_runlevel_info function.
 */
static
int set_newlevel(struct runlevel *runlevelinfo, char code)
{
	if (runlevelinfo == NULL)
		return 1;

	if (!IS_VALID_LEVEL(code))
		return 1;

	runlevelinfo->code = code;
	runlevelinfo->dir = get_script_directory(code);

	if (runlevelinfo->dir == NULL)
		return 1;
	return 0;
}

/*
 * See set_runlevel_info function.
 */
static
int set_prevlevel(struct runlevel *runlevelinfo, char code)
{
	if (IS_VALID_LEVEL(code))
		return set_newlevel(runlevelinfo, code);
	else if (!IS_VALID_PREV_LEVEL(code))
		return 1;

	runlevelinfo->dir = NULL;
	runlevelinfo->code = code;
	return 0;
}

/*
 * See set_runlevel_info function.
 */
int set_newlevel_info(struct runlevel *newlevel)
{
	char *env_new = getenv("RUNLEVEL");

	if (env_new == NULL)
		return 1;

	return set_newlevel(newlevel, env_new[0]);
}

/*
 * See set_runlevel_info function.
 */
int set_prevlevel_info(struct runlevel *prevlevel)
{
	char *env_prev = getenv("PREVLEVEL");

	if (env_prev == NULL)
		return 1;

	return set_prevlevel(prevlevel, env_prev[0]);
}

/*
 * Check if prevlevel and actlevel are valid runlevels.
 *
 * Returns an integer equal to zero if runlevels are valid, or
 * an integer equal to one otherwise.
 */
int check_runlevels(char *prevlevel, char *actlevel)
{
	/* NULL is not a good runlevel */
	if (prevlevel == NULL || actlevel == NULL)
		return 1;
	/* Runlevel code is only one character */
	if (!(IS_VALID_PREV_LEVEL(prevlevel[0]) &&
	      IS_VALID_LEVEL(actlevel[0])))
		return 1;

	return 0;
}

/*
 * Free the memory of the elements contained by runlevelinfo.
 * The pointers are marked as null, and the integer values are
 * set to zero.
 */
void free_runlevel_items(struct runlevel *runlevelinfo)
{
	if (runlevelinfo == NULL)
		return;

	if (runlevelinfo->dir != NULL) {
		free(runlevelinfo->dir);
		runlevelinfo->dir = NULL;
	}

	runlevelinfo->code = 0;
}

/*
 * Free the memory of runlevelinfo and elements.
 * The pointers are marked as null, and the integer values are
 * set to zero.
 */
void free_runlevel(struct runlevel **runlevelinfo)
{
	if (*runlevelinfo == NULL)
		return;

	free_runlevel_items(*runlevelinfo);
	free(*runlevelinfo);
	*runlevelinfo = NULL;
}
