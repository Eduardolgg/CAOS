#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "../src/screen_print.h"

void print_usage()
{
	printf("Unit tests to screen_print.c\n");
	printf("   please use correct param to call tests\n");
	exit(1);
}

void close_files(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void run_print_function(char *option)
{
	switch(*option) {
	case 'a':
		print_inf_msg("%s", "PRINT_INF_MSG");
		break;
	case 'b':
		print_inf_msg_ln("%s", "PRINT_INF_MSG_LN");
		break;
	case 'c':
		print_war_msg("%s" ,"PRINT_WAR_MSG");
		break;
	case 'd':
		print_war_msg_ln("%s", "PRINT_WAR_MSG_LN");
		break;
	case 'e':
		print_err_msg("%s", "PRINT_ERR_MSG");
		break;
	case 'f':
		print_err_msg_ln("%s", "PRINT_ERR_MSG_LN");
		break;
	case 'g':
		print_err_msg_ln("%s", "PRINT_TEXT_MSG_LN");
		break;
	case 'h':
		print_err_msg_ln("%s", "PRINT_TEXT_MSG_LN");
		break;
	default:
		print_usage();
	}
}

int check_text_output(char *option, char *buffer)
{
	char *expected = NULL;
	char *result = NULL;
	switch(*option) {
	case 'a':
		expected = "PRINT_INF_MSG";
		break;
	case 'b':
		expected = "PRINT_INF_MSG_LN";
		break;
	case 'c':
		expected = "PRINT_WAR_MSG";
		break;
	case 'd':
		expected = "PRINT_WAR_MSG_LN";
		break;
	case 'e':
		expected = "PRINT_ERR_MSG";
		break;
	case 'f':
		expected = "PRINT_ERR_MSG_LN";
		break;
	case 'g':
		expected = "PRINT_TEXT_MSG_LN";
		break;
	case 'h':
		expected = "PRINT_TEXT_MSG_LN";
		break;
	default:
		return 1;
	}
	result = strstr(buffer, expected);
	if (result != NULL)
		return 0;
	else
		return 1;
}

int check_colorized_output(char *option, char *buffer)
{
	int is_text_present = !check_text_output(option, buffer);
	char *result = strstr(buffer, C_RESET);
	printf("%i-%s", is_text_present, result);
	if (is_text_present && result != NULL)
		return 0;
	else
		return 1;
}

int check_normal_output(char *option, char *buffer)
{
	return check_text_output(option, buffer);
}

int is_expected_output(char *option, char *buffer)
{
#ifdef USE_COLORS
	return check_colorized_output(option, buffer);
#else
	return check_normal_output(option, buffer);
#endif
}

int main(int argc, char **argv)
{
	pid_t pid;
	char buffer[50];
	int fd[2];

	if (argc < 2)
		print_usage();

	pipe(fd);
	switch (pid = fork()) {
	case 0:
		dup2(fd[1], STDOUT_FILENO);
		run_print_function(argv[1]);
		close_files(fd);
		break;
	case 1:
		printf("Fork error");
		break;
	default:
		/* Wait for the child to complete the work before
		   starting the tests and issue a result. */
		waitpid(pid, NULL, 0);

		dup2(fd[0], STDIN_FILENO);
		read(fd[0], buffer, 50);
		int result = is_expected_output(argv[1], buffer);
		close_files(fd);
		return result;
	}
	return 0;
}
