#ifndef SRC_CONSOLA_CONSOLA_H_
#define SRC_CONSOLA_CONSOLA_H_
/*
#include <stdio.h>
#include <stdlib.h>


#include <commons/string.h>
#include <commons/log.h>

#include <initial_configuration/client_start.h>
*/
#include "planificadores/planificador.h"
#include <readline/readline.h>
#include <echo/echo.h>

void consola_interactiva(t_log* logger, t_conn* conn);

#endif /* SRC_CONSOLA_CONSOLA_H_ */
