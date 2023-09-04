/*
 * logger.h
 *
 *  Created on: Sep 3, 2023
 *      Author: utnso
 */

#ifndef SRC_LOGGER_LOGGER_H_
#define SRC_LOGGER_LOGGER_H_

#include <stdlib.h>
#include <commons/log.h>

t_log* create_logger(char* file, bool is_active_console, t_log_level log_level);

#endif /* SRC_LOGGER_LOGGER_H_ */
