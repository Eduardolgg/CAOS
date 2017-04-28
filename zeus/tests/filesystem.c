#include "log.h"
#include "filesystem.h"

int main(int argc, char **argv)
{
	struct script_list script_list;
	int list_len,
	    expected_list_len,
	    i;

	if (argc < 4) {
		print_err_msg("Usage: %s prev_runlevel act_runlevel "
			      "expected_list_len\n"
			      "	runlevel codes 2|3|4|5|S\n"
			      "	expected_list_len = num script to run from "
			      " prev to act.\n", argv[0]);
		return 1;
	}

	struct runlevel prevlevel = {
		.code = argv[1][0],
		.dir = get_script_directory(argv[1][0])
	};
	struct runlevel newlevel = {
		.code = argv[2][0],
		.dir = get_script_directory(argv[2][0])
	};
	expected_list_len = argv[3][0] - '0';

	list_len = get_change_init_scripts(&prevlevel, &newlevel,
				&(script_list.list));
	script_list.len = list_len;

	if (list_len == -1)
		print_err_msg("Error in get_change_init_script\n");

	for(i = 0; i < list_len; i++) {
		print_inf_msg("%s\n", script_list.list[i]->d_name);
	}

	free_script_list(&script_list);
	return !(list_len == expected_list_len);
}
