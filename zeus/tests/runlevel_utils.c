#include <stdio.h>
#include <stdlib.h>
#include "runlevel_utils.h"

void print_usage(char *appname)
{
	printf("Usage: app testcode");
	exit(1);
}

int main(int argc, char **argv)
{
	struct runlevel *prevrunlevel,
	                *newrunlevel;
	int result;

	if (argc < 2)
		print_usage(argv[0]);

	switch(argv[1][0]) {
	case 'a':
		return set_runlevel_info(prevrunlevel, newrunlevel);
		break;
	case 'b':
		prevrunlevel = (struct runlevel *) malloc(
		                                      sizeof(struct runlevel));
		newrunlevel = (struct runlevel *) malloc(
		                                      sizeof(struct runlevel));

		result = set_runlevel_info(prevrunlevel, newrunlevel);

		free_runlevel(&prevrunlevel);
		free_runlevel(&newrunlevel);
		break;
	case 'c':
		result = !check_runlevels(NULL, NULL) ||
		          check_runlevels("N", "S") ||
		         !check_runlevels("X", "1") ||
		         !check_runlevels("1", "X");
		break;
	case 'd':
		result = !set_newlevel_info(NULL);
		break;
	default:
		result =  1;
	}

	return result;
}
