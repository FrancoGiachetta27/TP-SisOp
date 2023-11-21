#ifndef SRC_PAGE_PAGE_H_
#define SRC_PAGE_PAGE_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <package/package.h>

typedef struct
{
    int page_number;
    int displacement;
    uint32_t pid;
} t_pag;

typedef struct
{
    int page_number;
    int displacement;
    uint32_t pid;
    uint32_t register_value;
} t_mov_out;

typedef struct
{
    int swap_block;
    int page_info_size;
    void* page_content;
} t_pag_swap;

t_pag *page_new(uint32_t pid, int page_number, int displacement);
t_pag_swap* page_for_swap(int swap_block, int page_info_size, void* page_content);
t_mov_out *page_for_mov_out(uint32_t pid, int page_number, int displacement, uint32_t register_value);
void send_page_for_swap(int op_code, t_pag_swap* page, int client_socket, t_log* logger);
void send_page_for_mov_out(int op_code, t_mov_out *page, int client_socket, t_log *logger);
void send_page(int op_code, t_pag *page, int client_socket, t_log *logger);
t_pag *receive_page(int client_socket, t_log *logger);
t_mov_out *receive_page_for_mov_out(int client_socket, t_log *logger);
t_pag_swap* receive_page_for_swap(int client_socket, t_log* logger);

#endif /* SRC_PAGE_PAGE_H_ */
