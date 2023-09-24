#ifndef SRC_PCB_PCB_H_
#define SRC_PCB_PCB_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint32_t AX;
    uint32_t BX;
    uint32_t CX;
    uint32_t DX;
} t_reg;

enum ESTADO {
    BLOCKED = 1,
    NEW = 2,
    EXIT = 3,
    EXEC = 4,
    READY = 5
};

typedef struct pcb {
    uint32_t pid;
    uint32_t tamanio;
    uint32_t programCounter;
    uint32_t prioridad;
    char* nom_arch_inst;
    t_reg registers;
    int estado;
} t_pcb;

t_pcb* crear_pcb(uint32_t pid, char* name, uint32_t tamanio, uint32_t prioridad);
void* serializar_proceso_nuevo(uint32_t id, uint32_t tamanio, char* nombre);
void* serialize_pcb(t_pcb* pcb);
void* serialize_registers(t_reg registers);
t_reg deserialize_registers(void* buffer);
t_pcb* deserialize_pcb(void* buffer);
void destroy_pcb(t_pcb* pcb);

#endif /* SRC_PCB_PCB_H_ */
