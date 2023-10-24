#ifndef SRC_USER_SPACE_PAGES_PAGES_H
#define SRC_USER_SPACE_PAGES_PAGES_H

#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/memory_config.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include <command/command.h>

typedef struct {
    int frame;
    short int bit_precense;
    short int bit_modified;
    t_temporal* reference_instant;
    int swap_position;
} t_page;


t_list* page_table_create(process_bytes);
void send_page_size_to_cpu(t_conn* conn, t_utils* utils);

#endif /* SRC_USER_SPACE_PAGES_PAGES_H */