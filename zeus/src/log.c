#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "log.h"

#define vprint_chunk(writed_chars, buffer, var_args) \
	(writed_chars >=0) ? vprintf(buffer, var_args) : -1

#define print_chunk(writed_chars, format, color, ...) \
	(writed_chars >= 0) ? printf(format, color, ##__VA_ARGS__) : -1

/*
 * Color printing on the screen.
 *
 * This function prints color text on the screen, if the message incorporates a
 * line break at string end is necessary to include a reset code before the line
 * break to avoid unexpected behavior.
 *
 * Parameters:
 * format: see printf(3)
 * color: Color code. See log.h en include directory.
 *
 * Return value: Number of written characters, -1 if something went wrong.
 *
 * Bug: In case the line break character is set as a parameter this will
 * not be detected and the output may not be as expected.
 */
int print_msg(char *format, char *color, ...)
{
	/* \n to break, \0 don't break */
	char break_line = '\0';
	int final_char_pos,
	    writed_chars;
	va_list var_args;
	char *buffer = NULL;

	writed_chars = print_chunk(0, "%s", color);
	final_char_pos = strlen(format) - 1;
	buffer = (char *) malloc((sizeof(char) * final_char_pos + 2));
	strcpy(buffer, format);

	if (format[final_char_pos] == '\n') {
		buffer[final_char_pos] = '\0';
		break_line = '\n';
	}

	va_start(var_args, color);
	writed_chars += vprint_chunk(writed_chars, buffer, var_args);
	writed_chars += print_chunk(writed_chars, "%s%c", C_RESET, break_line);
	va_end(var_args);
	free(buffer);
	return writed_chars;
}
