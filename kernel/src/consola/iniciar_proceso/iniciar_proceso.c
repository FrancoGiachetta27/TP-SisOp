#include "iniciar_proceso.h"

uint32_t sig_PID;

void iniciar_proceso(char* source, char* size, char* priority, t_log* logger, t_conn* conn) {
    t_pcb* pcbCreado = crear_proceso(source, size, priority);
	avisar_a_memoria_nuevo_proceso(pcbCreado->pid, pcbCreado->tamanio, source, logger, conn);
}

void avisar_a_memoria_nuevo_proceso(uint32_t id, uint32_t tamanio, char* nombre,t_log* logger, t_conn* conn){
	/*char* nombreArchivo = string_from_format("%s.%s",nombre, "txt" );
	printf("\n%s", nombreArchivo);
	t_package* nuevoPaquete = create_empty_package(CREATE_PROCESS);
	void* buffer = serializar_proceso_nuevo(id, tamanio, nombre);
	int tamanioBuffer = 2 * sizeof(uint32_t) + string_length(nombre) + 1;
	nuevoPaquete->buffer = buffer;
	send_package(nuevoPaquete, conn->memory_socket, logger);
	destroy_package(nuevoPaquete);*/
};

void consultar_con_memoria_espacio_disp() {}

t_pcb* crear_proceso(char* source, char* tamanio, char* prioridad){
	consultar_con_memoria_espacio_disp();
	t_pcb* nuevoPCB = crear_pcb(sig_PID, source, atoi(tamanio), atoi(prioridad));
	agregar_pcb_a_cola_NEW(nuevoPCB);
	sem_post(&nuevoPcbEnSist);
	return nuevoPCB;
}

			