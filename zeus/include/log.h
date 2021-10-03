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
#include <string.h>
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
	#define C_PURPLE "\E[35m"
#else
	#define C_RESET ""
	#define C_GRAY ""
	#define C_GREEN ""
	#define C_YELLOW ""
	#define C_RED ""
	#define C_PURPLE ""
#endif

/*
 * Message type definitions.
 */
#define TEXT C_GRAY
#define INF C_GREEN
#define WAR C_YELLOW
#define ERR C_RED
#define DBG C_PURPLE

/*
 * Message print macros.
 */
int print_msg(char *format, char *color, ...);

#define print_text_msg(format, ...) print_msg(format, TEXT, ##__VA_ARGS__);

#define print_inf_msg(format, ...) print_msg(format, INF, ##__VA_ARGS__);

#define print_war_msg(format, ...) print_msg(format, WAR, ##__VA_ARGS__);

#define print_err_msg(format, ...) print_msg(format, ERR, ##__VA_ARGS__);

#ifdef DEBUG
#define print_dbg_msg(format, ...) print_msg(format, DBG, ##__VA_ARGS__); syslog_info(format, ##__VA_ARGS__)
#else
#define print_dbg_msg(format, ...) ({})
#endif

#define print_current_error_msg(format, ...)                                 \
({                                                                           \
	print_msg(format " %s\n", ERR, ##__VA_ARGS__, strerror(errno));    \
	openlog(APP_NAME, LOG_ODELAY, LOG_DAEMON);                           \
	syslog(LOG_ERR, format, ##__VA_ARGS__);                              \
	closelog();                                                          \
})

#define print_current_error()                                                  \
({                                                                             \
	print_current_error_msg("%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__);\
})

/*
 * Syslog messages
 */
#define syslog_info(format, ...)                    \
({                                                  \
	openlog(APP_NAME, LOG_ODELAY, LOG_DAEMON);  \
	syslog(LOG_INFO, format, ##__VA_ARGS__);    \
	closelog();                                 \
})

/*
 * Print CAOS init suite information
 */
#define PRINT_APP_INFO	                                      \
({                                                            \
	print_inf_msg("%s: System is boot up\n", CAOS_INFO);  \
	print_text_msg(CAOS_BANNER);                          \
})
