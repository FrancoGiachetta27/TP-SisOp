#ifndef SRC_USER_MEMORY_PAGE_ENTRY_H_
#define SRC_USER_MEMORY_PAGE_ENTRY_H_

#include <stdint.h>

typedef struct
{
    uint32_t pid;
    int page_number;
    int frame_number;
    short int bit_precense;
    short int bit_modified;
    int swap_position;
} t_page_entry;

typedef struct
{
    uint32_t process_pid;
    t_list *pages;
} t_page_table;

#endif /* SRC_USER_MEMORY_PAGE_ENTRY_H_ */