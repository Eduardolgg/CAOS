#pragma once

/*
 * Filesistem access.
 *
 *    24-Apr-2017 Elgg
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

#include <sys/wait.h>

#include "script_config.h"

struct proc_info {
	int fd;
	int fd_slave;
	char * script_name;
	pthread_t thread;
	int is_thread_end;
	int is_interactive;
	struct proc_info * prev;
	struct proc_info * next;
};

#define NORMAL_EXIT(status) WIFEXITED(status)
#define ERROR_EXIT(status) WEXITSTATUS(status) != 0

struct proc_info* malloc_proc_info(char *script_name);

void free_proc_info(struct proc_info **process);
