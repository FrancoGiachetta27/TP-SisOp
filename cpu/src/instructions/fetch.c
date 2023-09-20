#include <instructions/fetch.h>

char* fetch(int* program_counter, int pid, int memory_socket, t_log* logger) {
    t_package* package = create_empty_package(FETCH_INSTRUCTION);
    void* serialized_fetch = serialize_fetch(*program_counter, pid);
    package->buffer = serialized_fetch; 
    send_package(package, memory_socket, logger);
    char* instruction = receive_buffer(memory_socket, logger);
    free(serialized_fetch);
    if (instruction != NULL) {
        *program_counter++;
    }
    return instruction;
}

void* serialize_fetch(int program_counter, int pid) {
    void* buffer = malloc(sizeof(int) * 4);
	int desplazamiento = 0;
	memcpy(buffer + desplazamiento, sizeof(int), sizeof(int));
    desplazamiento+=sizeof(int);
	memcpy(buffer + desplazamiento, &pid, sizeof(int));
	desplazamiento+=sizeof(int);
    memcpy(buffer + desplazamiento, sizeof(int), sizeof(int));
	desplazamiento+=sizeof(int);
    memcpy(buffer + desplazamiento, &program_counter, sizeof(int));
    desplazamiento+=sizeof(int);
	return buffer;
} 