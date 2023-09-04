#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>

t_config* create_config(char* file, t_log* logger);
void check_if_config_value_exists(t_config* config, char* key, t_log* logger);

#endif /* SRC_CONFIG_CONFIG_H_ */
