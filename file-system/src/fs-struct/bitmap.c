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

// Cada entrada será representada por un dato de tipo uint32_t
// Bloque lógico 0 estará reservado y no debe ser asignado a ningún archivo.
// El valor correspondiente a un EOF (End of File) será representado como la constante UINT32_MAX.

int fd_fat;
int fd_block;
void *bitmap;
t_bitarray* bitarray;

/*
    FAT FUNCTIONS
*/

void create_fat_file(t_utils *utils) {

    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    size_t fat_size = (block_total - block_swap) * sizeof(uint32_t);

    char* path = config_get_string_value(utils->config, "PATH_FAT");

    fd_fat = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd_fat == -1) {
        log_error(utils->logger, "Error al crear el archivo de la tabla FAT");
        exit(1);
    }
    
    // Size en bytes
    ftruncate(fd_fat, fat_size);

    bitmap = mmap(NULL, fat_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fat, 0);

    if (bitmap == MAP_FAILED) {
        log_error(utils->logger, "No se pudo mapear el archivo %s", path);
        close(fd_fat);
        exit(1);
    }

    // TODO: Chequear si el file fue creado o abierto -> creado, llenar todo a 0s
    memset(bitmap, 0, fat_size);
    msync(bitmap, fat_size, MS_SYNC);

    bitarray = bitarray_create_with_mode((char*) bitmap, fat_size , LSB_FIRST);

    // Check
    // munmap(bitmap, fat_size);
    // close(fd_fat);

    log_info(utils->logger, "Archivo de la tabla FAT creado con éxito");
}

u_int32_t allocate_block() {
    size_t bitarray_size = (int) bitarray_get_max_bit(bitarray);
    for (int i = 1; i < bitarray_size; i++) {
        if (!bitarray_test_bit(bitarray, i)) {
            bitarray_set_bit(bitarray, i);
            msync(bitarray->bitarray, bitarray->size, MS_SYNC);
            return i;
        }
    }
    return -1;
}

void leer_fat() {
    // // Print valores
    char* hex = mem_hexstring(bitmap, 3840);
    printf("HEX: %s\n", hex);
}

/*
=================================================================================
*/

void create_block_file(t_utils *utils) {
    int block_total = config_get_int_value(utils->config, "CANT_BLOQUES_TOTAL");
    int block_swap = config_get_int_value(utils->config, "CANT_BLOQUES_SWAP");
    int block_size = config_get_int_value(utils->config, "TAM_BLOQUE");

    size_t block_file_size = block_total * block_size;

    char* path = config_get_string_value(utils->config, "PATH_BLOQUES");

    fd_block = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd_block == -1) {
        log_error(utils->logger, "Error al crear el archivo de bloques");
        exit(1);
    }

    ftruncate(fd_block, block_file_size);

    void *block_data = mmap(NULL, block_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_block, 0);

    if (block_data == MAP_FAILED) {
        log_error(utils->logger, "No se pudo mapear el archivo de bloques %s", path);
        close(fd_block);
        exit(1);
    }

    // Asignar valores a swap y fat - particiones
    msync(block_data, block_file_size, MS_SYNC);

    // CHECK
    // munmap(block_data, block_file_size);
    // close(fd_block);

    log_info(utils->logger, "Archivo de bloques creado con éxito.");
}