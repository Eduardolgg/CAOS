/*
 * General config.
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

/*
 * CAOS init suite informaticon
 */
#define CAOS_NAME "CAOS"
#define CAOS_SHORT_DESC CAOS_NAME " init suite"
#define CAOS_VERSION "0.0.0"
#define CAOS_INFO CAOS_SHORT_DESC " " CAOS_VERSION

/*
 * Compilation constants.
 */
#define develop

#define USE_COLORS
/*#undef USE_COLRS*/

/*
 * Runlevel info.
 */
#define RUNLEVEL_NONE 'N'
#define RUNLEVEL_START 'S'

#define RCX_DIR "/etc/rcX.d"
#ifndef develop
	#define RCX_BUFFER_LEN 11
#else
	#define RCX_BUFFER_LEN 21
#endif

#define CAOS_BANNER  \
	"  ▄████▄   ▄▄▄       ▒█████    ██████     ██▓ ███▄    █  ██▓▄▄▄█████▓   \n" \
	"  ▒██▀ ▀█  ▒████▄    ▒██▒  ██▒▒██    ▒    ▓██▒ ██ ▀█   █ ▓██▒▓  ██▒ ▓▒  \n" \
	"  ▒▓█    ▄ ▒██  ▀█▄  ▒██░  ██▒░ ▓██▄      ▒██▒▓██  ▀█ ██▒▒██▒▒ ▓██░ ▒░  \n" \
	"  ▒▓▓▄ ▄██▒░██▄▄▄▄██ ▒██   ██░  ▒   ██▒   ░██░▓██▒  ▐▌██▒░██░░ ▓██▓ ░   \n" \
	"  ▒ ▓███▀ ░ ▓█   ▓██▒░ ████▓▒░▒██████▒▒   ░██░▒██░   ▓██░░██░  ▒██▒ ░   \n" \
	"  ░ ░▒ ▒  ░ ▒▒   ▓▒█░░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░   ░▓  ░ ▒░   ▒ ▒ ░▓    ▒ ░░     \n" \
	"    ░  ▒     ▒   ▒▒ ░  ░ ▒ ▒░ ░ ░▒  ░ ░    ▒ ░░ ░░   ░ ▒░ ▒ ░    ░      \n" \
	"  ░          ░   ▒   ░ ░ ░ ▒  ░  ░  ░      ▒ ░   ░   ░ ░  ▒ ░  ░        \n" \
	"  ░ ░            ░  ░    ░ ░        ░      ░           ░  ░             \n" \
	"  ░                                                                     \n" \

