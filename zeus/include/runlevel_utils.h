#pragma once

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

/*
 * Allows to manage runlevel data in one place.
 */
struct runlevel {
	char code;
	char *dir;
};

/*
 * Runlevel info.
 */
#define RUNLEVEL_NONE 'N'
#define RUNLEVEL_START 'S'
#define VALID_LEVELS "S0123456"
#define VALID_PREV_LEVELS  "N" VALID_LEVELS

/*
 * Check if system is booting.
 *
 * Return true if system is booting, false otherwise.
 */
#define IS_SYS_BOOT(prev, act) \
	prev == RUNLEVEL_NONE && act == RUNLEVEL_START

/*
 * Check for valid runlevels.
 *
 * Return true if system is booting, false otherwise.
 */
#define IS_VALID_LEVEL(code) (index(VALID_LEVELS, code) != NULL)
#define IS_VALID_PREV_LEVEL(code) (index(VALID_PREV_LEVELS, code) != NULL)

/*
 * Store runlevel information in prevlevel and newlevel
 * parameters. Runlevel information is obtained from enviroments
 * variables PREVLEVEL and RUNLEVEL see init(8) for details.
 *
 * Return 0 if runlevels can be set. On error return 1.
 */
int set_runlevel_info(struct runlevel *prevlevel, struct runlevel *newlevel);

/*
 * See set_runlevel_info function.
 */
int set_prevlevel_info(struct runlevel *prevlevel);
int set_newlevel_info(struct runlevel *newlevel);

/*
 * Check if prevlevel and actlevel are valid runlevels.
 *
 * Returns an integer equal to zero if runlevels are valid, or
 * an integer equal to one otherwise.
 */
int check_runlevels(char *prevlevel, char *actlevel);

/*
 * Free the memory of the elements contained by runlevelinfo.
 * The pointers are marked as null, and the integer values are
 * set to zero.
 */
void free_runlevel_items(struct runlevel *runlevelinfo);

/*
 * Free the memory of runlevelinfo and elements.
 * The pointers are marked as null, and the integer values are
 * set to zero.
 */
void free_runlevel(struct runlevel **runlevelinfo);
