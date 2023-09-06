#include <logger/logger.h>

t_log* create_logger(char* file, bool is_active_console, t_log_level log_level) {
	return log_create(file, "client", is_active_console, log_level);
}

