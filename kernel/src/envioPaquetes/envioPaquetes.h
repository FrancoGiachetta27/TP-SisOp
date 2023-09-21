/*
 * envioPaquetes.h
 *
 *  Created on: Sep 19, 2023
 *      Author: utnso
 */

#ifndef SRC_ENVIOPAQUETES_ENVIOPAQUETES_H_
#define SRC_ENVIOPAQUETES_ENVIOPAQUETES_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <stdint.h>

void* serializar_proceso_nuevo(uint32_t id, uint32_t tamanio, char* nombre);

#endif /* SRC_ENVIOPAQUETES_ENVIOPAQUETES_H_ */
