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
void assign_block(int file_block, int free_block);
void set_end_of_file(int file_block);
void print_fat();
void assign_block_size(int file_size);

// Bloques
void create_block_file();
void initialize_swap_list();
void print_swap();
// bool reserve_swap_blocks(int);
t_list* reserve_swap_blocks(int);
int find_free_swap_block();
void free_swap_blocks(t_list* blocks_to_release);

#endif /* FAT_BLOQUE_H_ */