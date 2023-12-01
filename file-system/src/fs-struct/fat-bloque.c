#include "fat-bloque.h"

extern t_utils *utils;
extern t_fs_config fs_config;
int fd_fat;
int fd_block;
uint32_t *fat_data;
void *block_map;

// FAT
t_list *fat_list;
// Archivo de bloques

/*
    TODO
        - Revisar el uso de mmap para escribir cuando actualizo las listas
        - Revisar tamanio de bloque de fat != bloque archivos
        - Ver bien valgrind
        - Destruir listas, desmapear y cerrar archivos
*/

// FAT

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

    log_debug(utils->logger, "Para un archivo de %d bytes, necesito %d bloques. Le agrego uno más y tiene que ser UINT32_MAX", file_size, blocks_needed);

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

// TODO: USAR MMAP
void add_blocks(int initial_block, int additional_blocks)
{

    // FILE *file = fopen(fs_config.path_fat, "rb+");
    // if (file == NULL)
    // {
    //     perror("Error al abrir/crear el archivo FAT");
    //     return;
    // }

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

    // // Actualizar la tabla FAT en el archivo
    // fseek(file, 0, SEEK_SET);
    // for (int i = 0; i < list_size(fat_list); i++)
    // {
    //     uint32_t block_value = (uint32_t)list_get(fat_list, i);
    //     fwrite(&block_value, sizeof(uint32_t), 1, file);
    // }

    // fclose(file);
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

// TODO: USAR MMAP
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

    fat_data = mmap(NULL, fat_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fat, 0);

    if (fat_data == MAP_FAILED)
    {
        log_error(utils->logger, "No se pudo mapear el archivo %s", path);
        close(fd_fat);
        exit(1);
    }

    // TODO: Chequear si el file fue creado o abierto -> creado, llenar todo a 0s
    memset(fat_data, 0, fat_size);
    msync(fat_data, fat_size, MS_SYNC);

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
ARCHIVO DE BLOQUES
SWAP + BLOQUES FAT
*/

// SWAP

void create_block_file()
{
    int block_total = fs_config.block_total_count;
    int block_swap = fs_config.block_swap_count;
    int block_size = fs_config.block_size;

    // Size en bytes
    size_t block_file_size = block_total * block_size;

    char *path = fs_config.path_block;

    fd_block = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (fd_block == -1)
    {
        log_error(utils->logger, "Error al crear el archivo de la tabla FAT");
        close(fd_block);
        exit(1);
    }

    ftruncate(fd_block, block_file_size);

    block_map = mmap(NULL, block_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_block, 0);

    if (block_map == MAP_FAILED)
    {
        log_error(utils->logger, "No se pudo mapear el archivo %s", path);
        close(fd_block);
        exit(1);
    }

    // Ver con que rellenar
    memset(block_map, '0', block_file_size);
    msync(block_map, block_file_size, MS_SYNC);

    log_info(utils->logger, "Archivo de bloques creado con éxito");
}

// Puedes utilizar el mismo block_map para gestionar el estado de los bloques de SWAP.
// Al iniciar el proceso, podrías reservar un rango específico de bloques en block_map
// para la SWAP (por ejemplo, los primeros bloques después de la partición de FAT).
// Al reservar un bloque, actualizarías el estado de ese bloque tanto en la lista especial
// como en block_map.
// Al finalizar un proceso, marcarías los bloques de SWAP como libres tanto en la lista como
// en block_map.
int find_free_swap_block()
{
    for (int i = 0; i < fs_config.block_swap_count; i++)
    {
        char *block = (char *)(block_map + i * fs_config.block_size);

        int is_free = 1;
        for (int j = 0; j < fs_config.block_size; j++)
        {
            if (block[j] != '0')
            {
                is_free = 0;
                break;
            }
        }

        if (is_free)
        {
            return i;
        }
    }

    return -1;
}

t_list *reserve_swap_blocks(int blocks_count)
{
    t_list *blocks_swap = list_create();

    if (blocks_count > fs_config.block_swap_count)
    {
        log_debug(utils->logger, "No hay tantos bloques en SWAP");
        return NULL;
    }

    for (int i = 0; i < blocks_count; i++)
    {
        int block = find_free_swap_block();
        log_debug(utils->logger, "Bloque libre de swap %d - Pasa a '\\0'", block);

        if (block == -1)
        {
            log_debug(utils->logger, "No se pudo encontrar un bloque libre en SWAP");
            break;
        }

        char *block_data = (char *)(block_map + block * fs_config.block_size);
        memset(block_data, '\0', fs_config.block_size);

        list_add(blocks_swap, block);
    }

    return blocks_swap;
}

void free_swap_blocks(t_list *blocks_to_release)
{
    if (blocks_to_release == NULL)
    {
        log_debug(utils->logger, "La lista de bloques a liberar es nula");
        return;
    }

    for (int i = 0; i < list_size(blocks_to_release); i++)
    {
        int block_index = *(int *)list_get(blocks_to_release, i);

        if (block_index < 0 || block_index >= fs_config.block_swap_count)
        {
            log_debug(utils->logger, "FREE - Índice de bloque no válido: %d", block_index);
            continue;
        }

        char *block_data = (char *)(block_map + block_index * fs_config.block_size);
        memset(block_data, '0', fs_config.block_size);
        log_debug(utils->logger, "Bloque ocupado de swap %d - Pasa a '0'", block_index);
    }
}

void write_to_swap_block(int block_index, void *data)
{
    if (block_index < 0 || block_index >= fs_config.block_swap_count)
    {
        log_debug(utils->logger, "WRITE - Índice de bloque no válido: %d", block_index);
        return;
    }

    if (data == NULL)
    {
        log_debug(utils->logger, "La cadena de datos es NULL");
        return;
    }

    void *block = block_map + block_index * fs_config.block_size;

    size_t data_length = strlen(data);
    size_t format_length = (data_length < fs_config.block_size) ? data_length : fs_config.block_size;

    memcpy(block, data, format_length);

    // rellenar bloque con \0 - hace falta?
    if (format_length < fs_config.block_size)
    {
        memset(block + format_length, '\0', fs_config.block_size - format_length);
    }

    // mem_hexdump(block_map, fs_config.block_total_count);

    log_debug(utils->logger, "Datos escritos en el bloque %d de la swap", block_index);
}

void *read_from_swap_block(int block_index)
{
    if (block_index < 0 || block_index >= fs_config.block_swap_count)
    {
        log_debug(utils->logger, "READ - Índice de bloque no válido: %d\n", block_index);
        return;
    }

    void *buffer = malloc(fs_config.block_size);

    void *block = block_map + block_index * fs_config.block_size;

    // Leo el bloque entero
    memcpy(buffer, block, fs_config.block_size);

    log_debug(utils->logger, "Datos leídos desde el bloque %d de la swap: %s", block_index, (char *)buffer);

    return buffer;
}

// BLOQUES FAT - INFO ARCHIVOS

// INIT

void init_fs_struct()
{
    create_fat_file();
    initialize_fat_list();
    create_block_file();
}

void destroy_fs()
{
    list_destroy(fat_list);

    munmap(block_map, fs_config.block_size * fs_config.block_total_count);

    close(fd_block);
    close(fd_fat);
}