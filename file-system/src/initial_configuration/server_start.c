#include <initial_configuration/server_start.h>

int start_server_port(t_utils* utils) {
	if (!check_if_config_value_exists(utils, "PUERTO_ESCUCHA")) {
		utils_destroy(utils);
		return -1;
	}
	int server_fd = start_server(config_get_string_value(utils->config, "PUERTO_ESCUCHA"), utils->logger);
	if (server_fd == -1) {
		utils_destroy(utils);
		return -1;
	}
	log_trace(utils->logger, "Se inicio correctamente el server");
	int initial_handshake_result = wait_for_initial_handshake_from_kernel(server_fd, utils->logger);
	if (initial_handshake_result == -1) {
		utils_destroy(utils);
		return -1;
	}
	return server_fd;
}
