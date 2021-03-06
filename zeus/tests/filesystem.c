#include <unistd.h>

#include "log.h"
#include "filesystem.h"

int main(int argc, char **argv)
{
	struct script_list script_list;
	int list_len,
	    expected_list_len,
	    i, result = 1;
	char cwd[1024];

	if (argc < 5) {
		print_err_msg("Usage: %s test_code prev_runlevel act_runlevel "
			      "expected_list_len\n"
		              " test_code: \n"
		              "\ta: Test init diff."
		              "\tb: Test wrong runlevel code."
		              "\tc: Test free script list."
		              "\td: Test script config."
			      "	runlevel codes 2|3|4|5|S\n"
			      "	expected_list_len = num script to run from "
			      " prev to act.\n", argv[0]);
		return 1;
	}
	struct runlevel prevlevel = {
		.code = argv[2][0],
		.dir = get_script_directory(argv[2][0])
	};
	struct runlevel newlevel = {
		.code = argv[3][0],
		.dir = get_script_directory(argv[3][0])
	};
	switch(argv[1][0]) {
	case 'a':
		expected_list_len = argv[4][0] - '0';

		list_len = get_change_init_scripts(&prevlevel, &newlevel,
					&(script_list.list));
		script_list.len = list_len;

		if (list_len == -1)
			print_err_msg("Error in get_change_init_script\n");

		for(i = 0; i < list_len; i++) {
			print_inf_msg("%s\n", script_list.list[i]->d_name);
		}

		free_script_list(&script_list);
		result = !(list_len == expected_list_len);
		break;
	case 'b':
		result = !(get_script_directory(argv[2][0]) == NULL);
		break;
	case 'c':
		list_len = get_change_init_scripts(&prevlevel, &newlevel,
					&(script_list.list));
		free_script_llist(&(script_list.list), list_len);
		result = !(script_list.list == NULL);
		break;
	case 'd':
		if (chdir(get_script_directory('S')) != 0)
			return 1;

		getcwd(cwd, sizeof(cwd));
		print_inf_msg("Current working dir: %s\n", cwd);

		result = is_user_interactive("allmonth.sh");
		result += is_user_interactive("non-existent.sh");
		result += is_user_interactive("spring.sh");
		result = !(result == 2);
		break;
	}
	return result;
}
