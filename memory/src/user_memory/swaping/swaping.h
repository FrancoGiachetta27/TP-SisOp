#ifndef SRC_USER_MEMORY_SWAPING_SWAPING_H
#define SRC_USER_MEMORY_SWAPING_SWAPING_H

#include <package/package.h>
#include <command/opcodes.h>
#include <user_memory/real_memory.h>
#include <page/page.h>
#include <commons/collections/list.h>
#include <commons/log.h>

void swap_in(t_page_entry *page_referenced, int frame_number, int fs_socket, t_log *logger);
void swap_out(t_page_entry *victim, int fs_socket, t_log *logger);
void free_swap_blocks(t_page_table* page_table, int fs_socket, t_log *logger);
t_list *get_swap_blocks(int bytes, int socket_fs, t_log *logger);

#endif /* SRC_USER_MEMORY_SWAPING_SWAPING_H */