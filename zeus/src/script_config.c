#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "toml.h"
#include "script_config.h"
#include "log.h"

#define TRUE 1
#define FALSE 0

int init_config_file_name(struct script_config* config)
{
	if (sizeof(config->script_name) + sizeof(SCRIPT_CONFIG_EXTENCION) >=
                                                         CONFIG_FILE_NAME_LEN) {
		print_err_msg("Error script config name is too long [%s]",
                                                           config->script_name);
		return FALSE;
	}

	memset(config->config_file_name, '\0', CONFIG_FILE_NAME_LEN);
	if (config->script_name[0] == 'S' || config->script_name[0] == 'K')
		strcpy(config->config_file_name, config->script_name + 3);
	else
		strcpy(config->config_file_name, config->script_name);
	strcat(config->config_file_name, SCRIPT_CONFIG_EXTENCION);
	return TRUE;
}

void init_config_file_path(struct script_config* config)
{
	memset(config->config_file_path, '\0', CONFIG_FILE_PATH_LEN);
	strcat(config->config_file_path, CAOS_CONFIG_DIR);
	strcat(config->config_file_path, config->config_file_name);
}

int read_script_configuration(toml_table_t** conf_data, struct script_config* config)
{
	FILE *file;
	char errbuf[200];

	if ((file = fopen(config->config_file_path, "r")) == 0) {
		print_err_msg("Error opening config file %s error: %s\n",
		   config->config_file_name, strerror(errno));
		return FALSE;
	}

	*conf_data = toml_parse_file(file, errbuf, sizeof(errbuf));
	if (conf_data == 0) {
		print_err_msg("Error parsing config file %s error: %s\n",
		   config->config_file_name, errbuf);
		return FALSE;
	}
	fclose(file);
	return TRUE;
}

int is_interactive(toml_table_t* lsb)
{
	int is_interactive;
	const char* raw;

	if ((raw = toml_raw_in(lsb, "user-interactive")) == 0)
		return FALSE;

	if (toml_rtob(raw, &is_interactive)) {
		print_err_msg("Error: user-interactive bad value\n");
		return FALSE;
	}

	return is_interactive;
}

int get_lsb_configuration(toml_table_t* conf_data, struct script_config* config)
{
	toml_table_t* lsb;

	if ((lsb = toml_table_in(conf_data, "lsb")) == 0) {
		print_err_msg("Error reading lsb config in %s\n", config->config_file_name);
		return FALSE;
	}

	config->lsb.user_interactive = is_interactive(conf_data);

	toml_free(lsb);
	return TRUE;
}

int read_script_config(struct script_config* config)
{
	toml_table_t* conf_data;
	int error = FALSE;

	if (config == NULL)
		return FALSE;
	if (!init_config_file_name(config))
		return FALSE;
	init_config_file_path(config);

	if(!read_script_configuration(&conf_data, config))
		return FALSE;

	//TODO: read init-script table from config file:

	if(!get_lsb_configuration(conf_data, config)) {
		error = TRUE;
		goto exit;
	}
exit:
	toml_free(conf_data);
	return !error;
}

void free_script_config(struct script_config* config)
{
	if (config != NULL)
		free(config);
}
