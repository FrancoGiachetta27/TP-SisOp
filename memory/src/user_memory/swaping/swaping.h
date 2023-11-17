#ifndef SRC_USER_MEMORY_REAL_MEMORY_H
#define SRC_USER_MEMORY_REAL_MEMORY_H

#include <command/command.h>

void swap_in(t_page_entry* page_referenced, int frame_number, int fs_socket, t_log* logger);
void swap_out(t_page_entry* victim, int fs_socket, t_log* logger);
int get_swap_blocks(int bytes, int socket_fs, t_log* logger);

#endif /* SRC_USER_MEMORY_REAL_MEMORY_H */