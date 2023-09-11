#ifndef SRC_CONSOLA_CONSOLA_H_
#define SRC_CONSOLA_CONSOLA_H_

#include <stdlib.h>
#include <stdio.h>
#include "consola.h"
#include <readline/readline.h>
#include <commons/string.h>
#include <commons/log.h>
#include <echo/echo.h>
#include <initial_configuration/client_start.h>

void consola_interactiva(t_log* logger, t_conn* conn);

#endif /* SRC_CONSOLA_CONSOLA_H_ */
