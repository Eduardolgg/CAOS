/*
 * Functions to print screen.
 *
 *    11-Apr-2017 Elgg
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

#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include "config.h"

/*
 * Color definitions.
 */
#ifdef USE_COLORS
	#define C_RESET "\E[0m"
	#define C_GRAY "\E[35m"
	#define C_GREEN "\E[32m"
	#define C_YELLOW "\E[33m"
	#define C_RED "\E[31m"
#else
	#define C_RESET ""
	#define C_GRAY ""
	#define C_GREEN ""
	#define C_YELLOW ""
	#define C_RED ""
#endif

/*
 * Message type definitions.
 */
#define TEXT "C_GRAY"
#define INF "C_GREEN"
#define WAR "C_YELLOW"
#define ERR "C_RED"

/*
 * Message macros. Please don't use uppercase macros in code,
 * create new like print_text_msg, etc.
 */
#define PRINT_MSG_GENERIC(msg, lineEnd, color, ...)  \
({                                                   \
	printf("%s", color);                         \
	printf(msg, ##__VA_ARGS__);                  \
	printf("%s%s", C_RESET, lineEnd);            \
})

#define PRINT_MSG(msg, color, ...) \
	PRINT_MSG_GENERIC(msg, "", color, ##__VA_ARGS__);
#define PRINT_MSG_LN(msg, color, ...) \
	PRINT_MSG_GENERIC(msg, "\n", color, ##__VA_ARGS__);

/*
 * Message print macros.
 */
#define print_text_msg(msg, ...) PRINT_MSG(msg, C_GRAY, ##__VA_ARGS__);
#define print_text_msg_ln(msg, ...) PRINT_MSG_LN(msg, C_GRAY, ##__VA_ARGS__);

#define print_inf_msg(msg, ...) PRINT_MSG(msg, C_GREEN, ##__VA_ARGS__);
#define print_inf_msg_ln(msg, ...) PRINT_MSG_LN(msg, C_GREEN, ##__VA_ARGS__);

#define print_war_msg(msg, ...) PRINT_MSG(msg, C_YELLOW, ##__VA_ARGS__);
#define print_war_msg_ln(msg, ...) PRINT_MSG_LN(msg, C_YELLOW, ##__VA_ARGS__);

#define print_err_msg(msg, ...) PRINT_MSG(msg, C_RED, ##__VA_ARGS__);
#define print_err_msg_ln(msg, ...) PRINT_MSG_LN(msg, C_RED, ##__VA_ARGS__);

#define print_current_error()                       \
({                                                  \
	PRINT_MSG_LN(strerror(errno), C_RED);       \
	openlog(APP_NAME, LOG_ODELAY, LOG_DAEMON);  \
	syslog(LOG_ERR, "%s", strerror(errno));     \
	closelog();                                 \
})
#define print_current_error_msg(msg, ...)           \
({      /* TODO: Too long for a macro? :-/ */       \
	PRINT_MSG(msg, C_RED, ##__VA_ARGS__);       \
	PRINT_MSG_LN(strerror(errno), C_RED);       \
	openlog(APP_NAME, LOG_ODELAY, LOG_DAEMON);  \
	syslog(LOG_ERR, msg, ##__VA_ARGS__);        \
	closelog();                                 \
})

/*
 * Print CAOS init suite information
 */
#define PRINT_APP_INFO	                         \
({                                               \
	print_inf_msg_ln("System is boot up ");  \
	print_text_msg_ln(CAOS_BANNER);          \
})
