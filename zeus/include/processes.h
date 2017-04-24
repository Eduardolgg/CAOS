#include <sys/wait.h>

#define NORMAL_EXIT(status) WIFEXITED(status)
#define ERROR_EXIT(status) WEXITSTATUS(status) != 0
