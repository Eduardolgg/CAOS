#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "run.h"
#include "log.h"
#include "config.h"

char *create_file_path(char *base_path, struct run_info *info)
{
	size_t len = strlen(base_path) + strlen(info->file_name) + 1;
	char * path = (char *) malloc(sizeof(char) * len);
	if (path == NULL)
		print_war_msg("I can't create the file path for [%s]: no memory allocated\n",
			      info->file_name);
	strcpy(path, base_path);
	strcpy(path + strlen(base_path), info->file_name);
	return path;
}

int create_zeus_run_directory()
{
	if (mkdir(PID_FILES_DIR, 0755) && errno != EEXIST) {
		print_war_msg("I can't create zeus run directory [%s]: %s\n",
			      PID_FILES_DIR, strerror(errno));
		return 1;
	}
	return 0;
}

void create_pid_file(struct run_info *info)
{
	FILE *pidfile;
	// If I can't create run directory, continuing is stupid.
	if (create_zeus_run_directory()) {
		print_war_msg("I can't create the pidfile I'm killable\n");
		return;
	}

	if ((pidfile = fopen(info->pid_file_path, "w")) == NULL) {
		print_war_msg("I can't create pidfile [%s]: %s\n",
			      info->pid_file_path, strerror(errno));
	} else if (!fprintf(pidfile, "%i", info->pid))
		print_war_msg("I can't write the pid %i to [%s]: %s\n",
			      info->pid, info->file_name, strerror(errno));
	fclose(pidfile);
}

void delete_pid_file(struct run_info *info)
{
	if (info->pid_file_path == NULL)
		print_war_msg("No pidfile for [%s]:\n", info->file_name);

	if (remove(info->pid_file_path))
		print_war_msg("I can't remove pidfile for [%s]: %s\n",
			     info->pid_file_path, strerror(errno));
}

void add_kill_ignore_link(struct run_info *info)
{
	if (symlink(info->pid_file_path, info->ignore_link_path))
		print_war_msg("I can't create ignore symlink for [%s]: %s\n",
			      info->pid_file_path, strerror(errno));
}

void delete_kill_ignore_link(struct run_info *info)
{
	if (info->ignore_link_path == NULL)
		print_war_msg("No symlink for [%s]:\n", info->file_name);

	if (remove(info->ignore_link_path))
		print_war_msg("I can't remove ignore symlink for [%s]: %s\n",
			     info->pid_file_path, strerror(errno));
}

#ifdef DEBUG

#define PRINT_WORKING_DIR_IF_DEBUG \
({\
	char cwd[256]; \
	if (getcwd(cwd, sizeof(cwd)) != NULL) \
		printf("Current working dir: %s\n", cwd); \
})
#else
#define PRINT_WORKING_DIR_IF_DEBUG ({})
#endif



void create_pid_file_and_kill_ignore(char *file_name, int pid,
				     struct run_info *info)
{
	PRINT_WORKING_DIR_IF_DEBUG;
	info->file_name = file_name;
	info->pid = pid;
	info->pid_file_path = create_file_path(PID_FILES_DIR, info);
	print_dbg_msg("Pidfile path [%s]\n", info->pid_file_path);
	info->ignore_link_path = create_file_path(SENDSIGS_OMIT_DIR, info);
	print_dbg_msg("Symlink file path [%s]\n", info->ignore_link_path);
	create_pid_file(info);
	add_kill_ignore_link(info);
}

void delete_pid_file_and_kill_ignore(char *file_name, struct run_info *info)
{
	PRINT_WORKING_DIR_IF_DEBUG;
	info->file_name = file_name;
	info->pid_file_path = create_file_path(PID_FILES_DIR, info);
	print_dbg_msg("Pidfile path [%s]\n", info->pid_file_path);
	info->ignore_link_path = create_file_path(SENDSIGS_OMIT_DIR, info);
	print_dbg_msg("Symlink file path [%s]\n", info->ignore_link_path);
	delete_kill_ignore_link(info);
	delete_pid_file(info);
}

void make_killall_ignore_me()
{
	struct run_info *info = (struct run_info *)
		malloc(sizeof(struct run_info));
	create_pid_file_and_kill_ignore("zeus", getpid(), info);

	free(info->pid_file_path);
	free(info);
}

void remove_killall_ignore()
{
	struct run_info *info = (struct run_info *)
		malloc(sizeof(struct run_info));
	delete_pid_file_and_kill_ignore("zeus", info);
	free(info->pid_file_path);
	free(info);
}
