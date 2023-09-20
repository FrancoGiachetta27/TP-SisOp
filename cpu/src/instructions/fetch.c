#include <instructions/fetch.h>

char* fetch(int* program_counter, int memory_socket, t_log* logger) {
    t_package* package = create_integer_package(0, *program_counter);
    send_package(package, memory_socket, logger);
    char* instruction = receive_buffer(memory_socket, logger);
    if (instruction != NULL) {
        *program_counter++;
    }
    return instruction;
}