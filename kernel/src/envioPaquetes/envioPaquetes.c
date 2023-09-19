#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>


void* serializar_proceso_nuevo(uint32_t id, uint32_t tamanio, char* nombre){
	void* bufferNuevo;
	int tamanioString = string_length(nombre);
	bufferNuevo = malloc(2*sizeof(uint32_t) + tamanioString);
	memcpy(bufferNuevo, &id, sizeof(uint32_t));
	memcpy(bufferNuevo, &tamanio, sizeof(uint32_t));
	memcpy(bufferNuevo, nombre, tamanioString);
	return bufferNuevo;
}


