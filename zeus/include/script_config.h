#include "config.h"

#define SCRIPT_CONFIG_EXTENCION ".toml"
#define CONFIG_FILE_NAME_LEN 256 + sizeof(SCRIPT_CONFIG_EXTENCION)  //Provar sizeof(struct dirent.d_name) para que tenga la misma longitud que tra el nombre del script al escanear los nombres.
#define CONFIG_FILE_PATH_LEN sizeof(CAOS_CONFIG_DIR) + CONFIG_FILE_NAME_LEN

struct lsb_data {
	int user_interactive;
};

struct script_config {
        char* script_name;
	char config_file_name[CONFIG_FILE_NAME_LEN];
	int config_file_name_len;
	char config_file_path[CONFIG_FILE_PATH_LEN];
	struct lsb_data lsb;
};

int read_script_config(struct script_config* config);
void free_script_config(struct script_config* config);
