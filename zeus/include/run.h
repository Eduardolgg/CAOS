
struct run_info {
	char *file_name;           // Nombre del script o proceso.
	int pid;                   // Pid del script en ejecución.
	char *pid_file_path;       // Path al pidfile.
	char *ignore_link_path;    // Path al link del ignorefile.
};

void make_killall_ignore_me();

void remove_killall_ignore();

//void create_pid_file_and_kill_ignore(char *file_name, int pid,
//				     struct run_info *info);

//void delete_pid_file_and_kill_ignore(struct run_info *info);
