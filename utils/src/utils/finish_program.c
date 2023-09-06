#include <utils/finish_program.h>

void finish_program_with_connection(t_log* logger, t_config* config, int conexion) {
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}

void finish_program(t_log* logger, t_config* config) {
	log_destroy(logger);
	config_destroy(config);
}
