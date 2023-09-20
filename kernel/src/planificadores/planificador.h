/*#include <stdlib.h>
#include <stdio.h>

#include <commons/string.h>

#include <config/config.h>

#include <initial_configuration/client_start.h>
#include <planificadores/codigosDeOperacion.h>

*/
#include <commons/collections/list.h>
#include "pcb.h"
#include <envioPaquetes/envioPaquetes.h>
#include <package/package.h>
#include <planificadores/codigosDeOperacion.h>

void consultar_con_memoria_espacio_disp();
uint32_t obtener_cantidad_dispositivos(char* dispositivos);
void iniciar_planificadores(t_utils* utils);
void avisar_a_memoria_nuevo_proceso(uint32_t id, uint32_t tamanio, char* nombre,t_log* logger, t_conn* conn);
t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad);
void prueba_agregar_proceso_a_NEW();
void listar_procesos_por_estado();
