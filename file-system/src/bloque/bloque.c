#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <config/config.h>
#include <commons/config.h>
#include <commons/memory.h>

void create_block_file(t_utils *utils) {
    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    size_t block_file_size = block_total * block_size;

    char* path = config_get_string_value(utils->config, "PATH_BLOQUES");

    int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1) {
        log_error(utils->logger, "Error al crear el archivo de bloques");
        exit(1);
    }

    ftruncate(fd, block_file_size);

    void *block_data = mmap(NULL, block_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (block_data == MAP_FAILED) {
        log_error(utils->logger, "No se pudo mapear el archivo de bloques %s", path);
        close(fd);
        exit(1);
    }

    // Asignar valores a swap y fat - particiones

    msync(block_data, block_file_size, MS_SYNC);
    munmap(block_data, block_file_size);

    close(fd);

    log_info(utils->logger, "Archivo de bloques creado con éxito.");
}