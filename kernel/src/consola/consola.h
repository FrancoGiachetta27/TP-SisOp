#ifndef SRC_CONSOLA_CONSOLA_H_
#define SRC_CONSOLA_CONSOLA_H_

#include "planificadores/planificador.h"
#include "iniciar_proceso/iniciar_proceso.h"
#include "finalizar_proceso/finalizar_proceso.h"
#include "listar_procesos/listar_procesos.h"
#include "multiprogramacion/multiprogramacion.h"
#include "detener_planificacion/detener_planificacion.h"
#include "iniciar_planificacion/iniciar_planificacion.h"
#include <readline/readline.h>
#include <echo/echo.h>

void consola_interactiva(t_log* logger, t_conn* conn);

#endif /* SRC_CONSOLA_CONSOLA_H_ */
