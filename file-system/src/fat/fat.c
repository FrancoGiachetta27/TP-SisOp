#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <config/config.h>
#include <commons/config.h>
#include <stdint.h>

void create_fat_file(t_utils *utils) {

    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    int tam_fat = (block_total - block_swap) * sizeof(uint32_t);

    char* path = config_get_string_value(utils->config, "PATH_FAT");

    int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1) {
        log_error(utils->logger, "Error al crear el archivo de la tabla FAT");
        exit(1);
    }

    uint32_t empty_block = 0;
    for (int i = 0; i < (block_total - block_swap); i++) {
        write(fd, &empty_block, sizeof(uint32_t));
    }

    close(fd);

    log_info(utils->logger, "Archivo de la tabla FAT creado con éxito");
}