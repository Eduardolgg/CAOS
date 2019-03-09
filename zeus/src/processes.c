/*
 * Processes manipulation.
 *
 *    06-Mar-2019 Elgg
 *
 *    This file is part of the CAOS init suite,
 *    Copyright (C) 2019 Eduardo L. García Glez.
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

#include <unistd.h>
#include <stdlib.h>

#include "processes.h"
#include "filesystem.h"

struct proc_info* malloc_proc_info(char *script_name)
{
	struct proc_info *p_info = NULL;

	p_info = (struct proc_info *) malloc(sizeof(struct proc_info));
	if (p_info) {
		p_info->fd = -1;
		p_info->fd_slave = -1;
		p_info->prev = NULL;
		p_info->next = NULL;
		p_info->script_name = script_name;
		p_info->is_interactive = is_user_interactive(script_name);
		p_info->is_thread_end = 0;
	}
	return p_info;
}

void free_proc_info(struct proc_info **process)
{
	if ((*process)->fd != -1)
		close((*process)->fd);
	if ((*process)->fd_slave != -1)
		close((*process)->fd_slave);
	/*if ((*process)->script_name)
		free((*process)->script_name);*/
}
