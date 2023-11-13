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
    uint32_t register_value;
    int page_number;
    int displacement;
    uint32_t pid;
} t_mov_out;

int size_of_page();
int size_of_page_for_mov_out();
t_pag *page_new(uint32_t pid, int page_number, int displacement);
t_mov_out *page_for_mov_out(uint32_t pid, int page_number, int displacement, uint32_t register_value);
void *serialize_page_for_mov_out(t_mov_out *page);
void *serialize_page(t_pag *page);
t_pag *deserialize_page(void *buffer);
t_mov_out *deserialize_page_for_mov_out(void *buffer);
void send_page_for_mov_out(int op_code, t_mov_out *page, int client_socket, t_log *logger);
void send_page(int op_code, t_pag *page, int client_socket, t_log *logger);
t_pag *receive_page(int client_socket, t_log *logger);
t_mov_out *receive_page_for_mov_out(int client_socket, t_log *logger);

#endif /* SRC_PAGE_PAGE_H_ */
