#ifndef SRC_UTILS_FINISH_PROGRAM_H_
#define SRC_UTILS_FINISH_PROGRAM_H_

#include <commons/config.h>
#include <commons/log.h>
#include <unistd.h>

void finish_program_with_connection(t_log* logger, t_config* config, int conexion);
void finish_program(t_log* logger, t_config* config);

#endif /* SRC_UTILS_FINISH_PROGRAM_H_ */
