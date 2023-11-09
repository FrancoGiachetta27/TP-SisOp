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

// FAT
t_list *fat_list;
// Archivo de bloques
t_list *swap_list;
t_list *fat_block_list;

void initialize_fat_list()
{
    fat_list = list_create();

    // Primer bloque siempre debe ser 0
    for (int i = 0; i < fs_config.block_total_count - fs_config.block_swap_count; i++)
    {
        list_add(fat_list, (uint32_t)0);
    }
}

int find_free_current_block()
{
    for (int i = 1; i < fs_config.block_total_count - fs_config.block_swap_count; i++)
    {
        if (list_get(fat_list, i) == 0 && i != 0)
        {
            return i;
        }
    }
    return -1;
}

int find_free_block(int current_block)
{
    for (int i = 1; i < fs_config.block_total_count - fs_config.block_swap_count; i++)
    {
        if (list_get(fat_list, i) == 0)
        {
            if (current_block != i && i != 0)
                return i;
        }
    }
    return -1;
}

void assign_block(int file_block, uint32_t free_block)
{
    list_replace(fat_list, file_block, free_block);
}

void set_end_of_file(int file_block)
{
    list_replace(fat_list, file_block, UINT32_MAX);
}

int assign_block_size(int file_size)
{
    FILE *file = fopen(fs_config.path_fat, "rb+");
    if (file == NULL)
    {
        perror("Error al abrir/crear el archivo FAT");
        return;
    }

    int current_block = find_free_current_block();
    int first_block = current_block;
    int blocks_assigned = 0;
    int blocks_needed = (file_size + sizeof(uint32_t) - 1) / sizeof(uint32_t);

    log_debug(utils->logger, "Para un archivo de %d bytes, necesito %d bloques. Le agrego uno más y tiene que ser UINT32_MAX\n", file_size, blocks_needed);

    while (blocks_assigned < blocks_needed)
    {

        int free_block = find_free_block(current_block);

        if (free_block == -1)
        {
            log_error(utils->logger, "Toda la FAT ocupada");
            break;
        }

        if (list_get(fat_list, current_block) == 0)
        {
            assign_block(current_block, free_block);
            // printf("Asigno a current_block %d el bloque libre %d\n", current_block, free_block);
        }

        current_block = free_block;
        blocks_assigned++;
    }

    // Marcar último bloque como UINT32_MAX
    set_end_of_file(current_block);

    // Actualizar la tabla FAT en el archivo
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < list_size(fat_list); i++)
    {
        uint32_t block_value = (uint32_t)list_get(fat_list, i);
        fwrite(&block_value, sizeof(uint32_t), 1, file);
    }

    fclose(file);
    return first_block;
}

int find_last_block(int initial_block)
{
    int current_block = initial_block;
    int last_block = current_block;

    while (current_block != UINT32_MAX)
    {
        last_block = current_block;
        current_block = list_get(fat_list, current_block);
    }

    return last_block;
}

void add_blocks(int initial_block, int additional_blocks)
{

    FILE *file = fopen(fs_config.path_fat, "rb+");
    if (file == NULL)
    {
        perror("Error al abrir/crear el archivo FAT");
        return;
    }

    int last_block = find_last_block(initial_block);

    int blocks_added = 0;

    while (blocks_added < additional_blocks)
    {
        int new_block = find_free_block(last_block);
        if (new_block == -1)
        {
            log_error(utils->logger, "Toda la FAT está ocupada.");
            break;
        }

        assign_block(last_block, new_block);
        last_block = new_block;
        blocks_added++;
    }

    set_end_of_file(last_block);

    // Actualizar la tabla FAT en el archivo
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < list_size(fat_list); i++)
    {
        uint32_t block_value = (uint32_t)list_get(fat_list, i);
        fwrite(&block_value, sizeof(uint32_t), 1, file);
    }

    fclose(file);
}

t_list *recorrer_blocks(int initial_block)
{
    t_list *blocks = list_create();

    // Agrego el primer bloque
    list_add(blocks, initial_block);

    int next_block = list_get(fat_list, initial_block);

    while (next_block != -1)
    {
        // printf("Bloque: %d - Valor %d\n", initial_block, next_block);
        list_add(blocks, next_block);
        initial_block = next_block;
        next_block = list_get(fat_list, initial_block);
    }

    return blocks;
}

void free_blocks(int initial_block, int blocks_needed)
{
    // descartando desde el final del archivo hacia el principio).
    t_list *blocks = recorrer_blocks(initial_block);

    for (int i = list_size(blocks) - 1; i > blocks_needed; i--)
    {
        printf("Bloques a liberar %d\n", list_get(blocks, i));
        list_replace(fat_list, list_get(blocks, i), 0);
    }

    list_replace(fat_list, blocks_needed + 1, UINT32_MAX);

    list_destroy(blocks);
}

void create_fat_file()
{
    int block_total = fs_config.block_total_count;
    int block_swap = fs_config.block_swap_count;

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

void print_fat()
{
    for (int i = 0; i < list_size(fat_list); i++)
    {
        uint32_t bloque = list_get(fat_list, i);
        if (bloque != 0)
        {
            log_debug(utils->logger, "Bloque %d - Valor: %d", i, bloque);
        }
    }
}

/*
=================================================================================
ARCHIVO DE BLOQUES
SWAP + FAT


*/

void initialize_swap_list()
{
    swap_list = list_create();

    for (int i = 0; i < fs_config.block_swap_count; i++)
    {
        // Lo lleno con 0s?
        list_add(swap_list, 0);
    }
}

int find_free_swap_block()
{
    for (int i = 0; i < list_size(swap_list); i++)
    {
        if (list_get(swap_list, i) == 0)
        {
            return i;
        }
    }
    return -1;
}

t_list *reserve_swap_blocks(int blocks_count)
{
    t_list *blocks_swap = list_create();

    if (blocks_count >= list_size(swap_list))
    {
        log_debug(utils->logger, "No hay tantos bloques en SWAP");
        return NULL;
    }

    for (int i = 0; i < blocks_count; i++)
    {
        int block = find_free_swap_block();
        log_debug(utils->logger, "Block libre de swap %d", block);
        list_replace(swap_list, block, 1);
        list_add(blocks_swap, block);
    }

    write_swap_file();

    return blocks_swap;
}

void free_swap_blocks(t_list *blocks_to_release)
{
    for (int i = 0; i < list_size(blocks_to_release); i++)
    {
        int block_to_release = list_get(blocks_to_release, i);
        if (block_to_release >= 0 && block_to_release < list_size(swap_list))
        {
            // Marcar el bloque como libre
            list_replace(swap_list, block_to_release, 0);
        }
    }

    write_swap_file();
}

void write_swap_file()
{
    int block_size = fs_config.block_size;

    FILE *file = fopen(fs_config.path_block, "wb"); // Abre el archivo en modo binario de escritura

    if (file == NULL)
    {
        return;
    }

    // Itera sobre la lista swap_list y escribe cada bloque en el archivo
    for (int i = 0; i < list_size(swap_list); i++)
    {
        uint32_t block_value = (uint32_t)list_get(swap_list, i);
        fwrite(&block_value, sizeof(uint32_t), 1, file);
    }

    fclose(file);
}

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
    msync(block_data, block_file_size, MS_SYNC);

    // CHECK
    // munmap(block_data, block_file_size);
    // close(fd_block);

    log_info(utils->logger, "Archivo de bloques creado con éxito.");
}

void print_swap()
{
    for (int i = 0; i < list_size(swap_list); i++)
    {
        uint32_t bloque = list_get(swap_list, i);
        if (bloque != 0)
        {
            log_debug(utils->logger, "Bloque swap %d - Valor: %d", i, bloque);
        }
        // log_debug(utils->logger, "Bloque swap %d - Valor: %d", i, bloque);
    }
}
