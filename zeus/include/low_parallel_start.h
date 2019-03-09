/*
 * Run init scripts in serial mode.
 *
 *    02-May-2017 Elgg
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

#include "runlevel_utils.h"

/*
 * Change from prev_level runlevel to new_level runlevel in parallel
 * mode, is based on the order given in the startup scripts.
 *
 * If prev_level code is equal to RUNLEVEL_NONE, all scripts in
 * new_runlevel are runned.
 *
 * The returned value is equal to zero if no error was detected, On
 * error return 1.
 *
 * Note: This function send status information to the screen
 * and to syslog.
 */
int low_parallel_start(struct runlevel *prev_level, struct runlevel *new_level);

/** Only for debug purposes */
struct proc_info* add_proc_item(struct proc_info **p_list,
				char* script_name);

void remove_queue_item(struct proc_info **process,
		       struct proc_info **head);
