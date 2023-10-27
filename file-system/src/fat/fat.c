#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <config/config.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <commons/memory.h>

// TODO: Agregar funcionalidades

// Cada entrada será representada por un dato de tipo uint32_t
// Bloque lógico 0 estará reservado y no debe ser asignado a ningún archivo.
// El valor correspondiente a un EOF (End of File) será representado como la constante UINT32_MAX.


void create_fat_file(t_utils *utils) {

    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    size_t fat_size = (block_total - block_swap) * sizeof(uint32_t);

    char* path = config_get_string_value(utils->config, "PATH_FAT");

    int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1) {
        log_error(utils->logger, "Error al crear el archivo de la tabla FAT");
        exit(1);
    }
    
    // Size en bytes
    ftruncate(fd, fat_size);

    void *bitmap = mmap(NULL, fat_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (bitmap == MAP_FAILED) {
        log_error(utils->logger, "No se pudo mapear el archivo %s", path);
        close(fd);
        exit(1);
    }

    // TODO: Chequear si el file fue creado o abierto -> creado, llenar todo a 0s
    // Llenar todos los bytes del bitmap a 0
    memset(bitmap, 0, fat_size);
    msync(bitmap, fat_size, MS_SYNC);

    // // Print valores
    // char* hex = mem_hexstring(bitmap, fat_size);
    // printf("HEX: %s\n", hex);


    munmap(bitmap, fat_size);
    
    close(fd);

    log_info(utils->logger, "Archivo de la tabla FAT creado con éxito");
}