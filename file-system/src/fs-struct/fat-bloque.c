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
#include <initial_configuration/fs_config.h>

// Cada entrada será representada por un dato de tipo uint32_t
// Bloque lógico 0 estará reservado y no debe ser asignado a ningún archivo.
// El valor correspondiente a un EOF (End of File) será representado como la constante UINT32_MAX.

extern t_utils *utils;
extern t_fs_config fs_config;
int fd_fat;
int fd_block;
void *bitmap;
t_bitarray *bitarray;
// Tabla FAT
uint32_t *fat_table; 
t_list *fat_list;

// NEW FAT

void initialize_fat_list()
{
    fat_list = list_create();

    // Primer bloque siempre 
    list_add(fat_list, 0);

    for (int i = 1; i < fs_config.block_total_count - fs_config.block_swap_count; i++)
    {
        list_add(fat_list, 0);
    }
}

int find_free_block()
{
    for (int i = 1; i < fs_config.block_total_count - fs_config.block_swap_count; i++)
    {
        printf("Bloque: %d, Valor: %d\n", i, list_get(fat_list, i));
        if (list_get(fat_list, i) == 0)
        {
            return i;
        }
    }
    return -1;
}

void assign_block(int file_block, int free_block)
{
    list_replace(fat_list, file_block, free_block);
}

void set_end_of_file(int file_block)
{
    list_replace(fat_list, file_block, UINT32_MAX);
}

void print_fat() {
    for (int i = 0; i < list_size(fat_list); i++) {
        uint32_t bloque = list_get(fat_list, i);
        if (bloque != 0) {
            log_debug(utils->logger, "Bloque %d - Valor: %d", i, bloque);
        }
    }
}

void assign_block_size(int file_size) {

    // Abrir el archivo FAT o crearlo si no existe
    FILE *file = fopen(fs_config.path_fat, "rb+");

    if (file == NULL)
    {
        perror("Error al abrir/crear el archivo FAT");
        return 1;
    }

    int current_block = 0;

    for (int i = 0; i < file_size; i++)
    {
        int free_block = find_free_block();

        // printf("Bloque %d\n", free_block);

        if (free_block == -1)
        {
            log_error(utils->logger, "Toda la FAT ocupada");
            break;
        }

        if (current_block == 0)
        {
            // Asignar el bloque inicial
            assign_block(0, 0);
        }
        else
        {
            // Asignar el bloque siguiente
            assign_block(current_block, free_block);
        }


        current_block = free_block;
        printf("Nuevo valor del bloque %d %d\n", current_block, list_get(fat_list, current_block));
    }

    // Marcar el último bloque como el final del archivo
    set_end_of_file(current_block);

    // Actuali la tabla FAT en el archivo
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < list_size(fat_list); i++) {
        uint32_t block_value = (uint32_t)list_get(fat_list, i);
        fwrite(&block_value, sizeof(uint32_t), 1, file);
    }

    fclose(file);
}

void create_fat_file()
{
    int block_total = fs_config.block_total_count;
    int block_swap = fs_config.block_swap_count;
    int block_size = fs_config.block_size;

    // Size en bytes
    size_t fat_size = (block_total - block_swap) * sizeof(uint32_t);

    // malloc?
    char *path = fs_config.path_fat;

    fd_fat = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd_fat == -1)
    {
        log_error(utils->logger, "Error al crear el archivo de la tabla FAT");
        close(fd_fat);
        exit(1);
    }

    ftruncate(fd_fat, fat_size);

    bitmap = mmap(NULL, fat_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fat, 0);

    if (bitmap == MAP_FAILED)
    {
        log_error(utils->logger, "No se pudo mapear el archivo %s", path);
        close(fd_fat);
        exit(1);
    }

    // TODO: Chequear si el file fue creado o abierto -> creado, llenar todo a 0s
    memset(bitmap, 0, fat_size);
    msync(bitmap, fat_size, MS_SYNC);   

    log_info(utils->logger, "Archivo de la tabla FAT creado con éxito");
}

/*
=================================================================================
*/

void create_block_file()
{
    int block_total = fs_config.block_total_count;
    int block_swap = fs_config.block_swap_count;
    int block_size = fs_config.block_size;

    size_t block_file_size = block_total * block_size;

    char *path = config_get_string_value(utils->config, "PATH_BLOQUES");

    fd_block = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd_block == -1)
    {
        log_error(utils->logger, "Error al crear el archivo de bloques");
        exit(1);
    }

    ftruncate(fd_block, block_file_size);

    void *block_data = mmap(NULL, block_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_block, 0);

    if (block_data == MAP_FAILED)
    {
        log_error(utils->logger, "No se pudo mapear el archivo de bloques %s", path);
        close(fd_block);
        exit(1);
    }

    memset(block_data, 0, block_file_size);

    // Asignar valores a swap y fat - particiones
    msync(block_data, block_file_size, MS_SYNC);

    // CHECK
    // munmap(block_data, block_file_size);
    // close(fd_block);

    log_info(utils->logger, "Archivo de bloques creado con éxito.");
}
