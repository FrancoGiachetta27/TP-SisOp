#ifndef SRC_USER_SPACE_MAIN_MEMORY_H
#define SRC_USER_SPACE_MAIN_MEMORY_H

#include <stdlib.h>
#include <initial_configuration/memory_config.h>
#include <commons/bitarray.h>

extern void* space;
extern t_bitarray* frame_table;

int check_available_frames(void);
void get_swap_blocks(int bytes,int socket_fs, t_log* logger);

#endif /* SRC_USER_SPACE_MAIN_MEMORY_H */