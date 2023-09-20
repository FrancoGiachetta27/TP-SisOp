#include "envioPaquetes.h"

void* serializar_proceso_nuevo(uint32_t id, uint32_t tamanio, char* nombre){
	void* bufferNuevo;
	int tamanioString = string_length(nombre) + 1;
	bufferNuevo = malloc(2*sizeof(uint32_t) + tamanioString  + sizeof(int));
	memcpy(bufferNuevo, &id, sizeof(uint32_t));
	memcpy(bufferNuevo, &tamanio, sizeof(uint32_t));
	memcpy(bufferNuevo, &tamanioString, sizeof(int));
	memcpy(bufferNuevo, nombre, tamanioString);
	return bufferNuevo;
}


