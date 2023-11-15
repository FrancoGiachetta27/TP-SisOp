#ifndef FAT_BLOQUE_H_
#define FAT_BLOQUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/log.h>
#include <config/config.h>
#include <initial_configuration/fs_config.h>

// FAT
void create_fat_file();
void initialize_fat_list();
int find_free_block(int);
int find_free_current_block();
int assign_block_size(int file_size);
void assign_block(int file_block, uint32_t free_block);
void set_end_of_file(int file_block);
void print_fat();
int find_last_block(int initial_block);
void add_blocks(int initial_block, int additional_blocks);
t_list *recorrer_blocks(int initial_block);
void free_blocks(int initial_block, int blocks_needed);

// Bloques
void create_block_file();
void initialize_swap_list();
void print_swap();
t_list *reserve_swap_blocks(int);
int find_free_swap_block();
void destroy_swap_list();
void free_swap_blocks(t_list *blocks_to_release);
void write_swap_file();

// Leer

#endif /* FAT_BLOQUE_H_ */