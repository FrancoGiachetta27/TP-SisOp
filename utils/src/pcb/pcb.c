#include "pcb.h"

t_reg create_empty_registers() {
	t_reg registers;
	registers.AX = 0;
	registers.BX = 0;
	registers.CX = 0;
	registers.DX = 0;
	return registers;
}

t_pcb* crear_pcb(uint32_t pid, char* name, uint32_t tamanio, uint32_t priority){
	t_pcb* nuevoPCB = malloc(sizeof(*nuevoPCB));
	nuevoPCB->pid = pid;
	nuevoPCB->tamanio = tamanio;
	nuevoPCB->programCounter = 0;
	nuevoPCB->prioridad = priority;
	nuevoPCB->nom_arch_inst = name;
	nuevoPCB->estado = NEW;
	nuevoPCB->registers=create_empty_registers();
	return nuevoPCB;
}

void send_pcb(int op_code, t_pcb* pcb, int client_socket, t_log* logger) {
	t_package* package = create_empty_package(op_code);
    package->size = serialized_pcb_size(pcb->nom_arch_inst);
    package->buffer = serialize_pcb(pcb);
    send_package(package, client_socket, logger);
}

t_pcb* receive_pcb(int client_socket, t_log* logger) {
	void* buffer = receive_buffer(client_socket, logger);
   	return deserialize_pcb(buffer);
}

int serialized_pcb_size(char* arch_name) {
	int arch_name_size = strlen(arch_name) + 1;
	return 8*sizeof(uint32_t) + sizeof(char) * arch_name_size + 2*sizeof(int);
}

void* serialize_pcb(t_pcb* pcb){
	int arch_name_size = strlen(pcb->nom_arch_inst) + 1;
	void* buffer = malloc(8*sizeof(uint32_t) + sizeof(char) * arch_name_size + 2*sizeof(int));
	void* buffer_registers = serialize_registers(pcb->registers);
	int offset = 0;
	memcpy(buffer + offset, &pcb->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &pcb->tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &pcb->programCounter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &pcb->prioridad, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &arch_name_size, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, pcb->nom_arch_inst, sizeof(char) * arch_name_size);
	offset += arch_name_size;
	memcpy(buffer + offset, buffer_registers, 4 * sizeof(uint32_t));
	offset += 4 * sizeof(uint32_t);
	free(buffer_registers);
	memcpy(buffer + offset, &pcb->estado, sizeof(int));
	return buffer;
}

void* serialize_registers(t_reg registers) {
	void* buffer = malloc(4*sizeof(uint32_t));
	int offset = 0;
	memcpy(buffer + offset, &registers.AX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &registers.BX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &registers.CX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(buffer + offset, &registers.DX, sizeof(uint32_t));
	return buffer;
}

t_reg deserialize_registers(void* buffer) {
	t_reg registers;
	int offset = 0;
	memcpy(&registers.AX, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&registers.BX, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&registers.CX, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&registers.DX, buffer + offset, sizeof(uint32_t));
	return registers;
}

t_pcb* deserialize_pcb(void* buffer) {
	t_pcb* pcb = malloc(sizeof(*pcb));
	int offset = 0;
	memcpy(&pcb->pid, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&pcb->tamanio, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&pcb->programCounter, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&pcb->prioridad, buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	int arch_name_size;
	memcpy(&arch_name_size, buffer + offset, sizeof(int));
	offset += sizeof(int);
	char* arch_name = malloc(sizeof(char) * arch_name_size);
	memcpy(arch_name, buffer + offset, sizeof(char) * arch_name_size);
	pcb->nom_arch_inst=arch_name;
	offset += arch_name_size;
	void* buffer_registers = malloc(4*sizeof(uint32_t));
	memcpy(buffer_registers, buffer + offset, 4 * sizeof(uint32_t));
	pcb->registers=deserialize_registers(buffer_registers);
	free(buffer_registers);
	offset += 4 * sizeof(uint32_t);
	memcpy(&pcb->estado, buffer + offset, sizeof(int));
	free(buffer);
	return pcb;
}

void destroy_pcb(t_pcb* pcb) {
	free(pcb->nom_arch_inst);
	free(pcb);
}