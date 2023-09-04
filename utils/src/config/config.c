#include <config/config.h>

t_config* create_config(char* file, t_log* logger)
{
	t_config* nuevo_config = config_create(file);
	if (nuevo_config == NULL) {
		log_error(logger, "No se encontro el archivo en %s", file);
		abort();
	}
	return nuevo_config;
}

void check_if_config_value_exists(t_config* config, char* key, t_log* logger)
{
	if (!config_has_property(config, key)) {
		log_error(logger, "No se encontro la propiedad %s en el archivo de config", key);
		abort();
	}
}

