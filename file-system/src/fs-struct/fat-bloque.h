#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/log.h>
#include <config/config.h>
#include <initial_configuration/fs_config.h>

// FAT
void initialize_fat_list();
int find_free_block(int);
int find_free_current_block();
void assign_block(int file_block, int free_block);
void set_end_of_file(int file_block);
void print_fat();
void assign_block_size(int file_size);

void create_fat_file();

// Bloques
void create_block_file();

#endif /* BITMAP_H_ */