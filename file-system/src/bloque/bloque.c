#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <config/config.h>
#include <commons/config.h>
#include <stdint.h>


// TODO: Usar mmap
void create_block_file(t_utils *utils) {
    // TODO: Ver si agregalo en una struct
    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    char* path = config_get_string_value(utils->config, "PATH_BLOQUES");

    // int tam_fat = (block_total - block_swap) * sizeof(uint32_t);

    int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1) {
        log_error(utils->logger, "Error al crear el archivo de bloques");
        exit(1);
    }

    // char block_data[block_size];
    // memset(block_data, 0, block_size); 
    // for (int i = 0; i < block_total; i++) {
    //     write(fd, block_data, block_size);
    // }

    close(fd);

    log_info(utils->logger, "Archivo de bloques creado con éxito.");
}