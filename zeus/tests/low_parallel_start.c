/*
 * Run init scripts in serial mode.
 *
 *    05-Mar-2019 Elgg
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

#include "low_parallel_start.h"
#include "processes.h"

int main(int argc, char **argv)
{
	struct proc_info *list_head = NULL;
	struct proc_info *list_end = NULL;

	switch(argv[1][0]) {
	case 'a': // Initialize procesess queue.
		list_end = add_proc_item(&list_head, "Irrelevant.sh");
		return !(list_head && list_end && list_end == list_head &&
		         list_head->next == list_head &&
			 list_head->prev == list_end);
	case 'b': // Add two items to processes queue.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		return !(list_head && list_end &&
		         list_end->prev == list_head &&
			 list_end->next == list_head &&
		         list_head->next == list_end &&
			 list_head->prev == list_end);
	case 'c': // Add three items to processes queue.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		list_end = add_proc_item(&list_head, "Irrelevant3.sh");
		return !(list_head && list_end &&
		         list_head->next->next == list_end &&
			 list_head->prev == list_end &&
		         list_end->prev->prev == list_head &&
			 list_end->next == list_head);
	case 'd': // Remove queue middle item.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		list_end = add_proc_item(&list_head, "Irrelevant3.sh");
		remove_queue_item(&(list_head->next), &(list_head));
		return !(list_head && list_end &&
		         list_end->prev == list_head &&
			 list_end->next == list_head &&
		         list_head->next == list_end &&
			 list_head->prev == list_end);
	case 'e': // Remove queue start item.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		list_end = add_proc_item(&list_head, "Irrelevant3.sh");
		struct proc_info *expected_head = list_head->next;
		remove_queue_item(&(list_head), &(list_head));
		return !(list_head && list_end &&
		         list_end->prev == list_head &&
		         list_head->next == list_end &&
			 list_head == expected_head &&
			 list_end == expected_head->next &&
			 list_end == expected_head->prev);
	case 'f': // Remove queue end item.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		list_end = add_proc_item(&list_head, "Irrelevant3.sh");
		struct proc_info *expected_end = list_end->prev;
		remove_queue_item(&(list_end), &(list_head));
		list_end = list_head->prev;
		return !(list_head && list_end &&
		         list_end->prev == list_head &&
		         list_head->next == list_end &&
			 list_head->prev == expected_end &&
			 list_end == expected_end &&
			 expected_end->next == list_head);
	case 'g': // Two queue list to one item.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		list_end = add_proc_item(&list_head, "Irrelevant2.sh");
		remove_queue_item(&(list_end), &(list_head));
		return !(list_head->prev == list_head &&
			 list_head->next == list_head);
	case 'h': // One queue list, remove item.
		list_end = add_proc_item(&list_head, "Irrelevant1.sh");
		remove_queue_item(&(list_end), &(list_head));
		return !(list_head == NULL);
	}
}
