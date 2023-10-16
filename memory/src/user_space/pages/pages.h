#ifndef SRC_PAGES_PAGES_H
#define SRC_PAGES_PAGES_H

#include <package/package.h>
#include <initial_configuration/server_start.h>
#include <initial_configuration/memory_config.h>
#include <command/command.h>

void send_page_size_to_cpu(t_conn* conn, t_utils* utils);

#endif /* SRC_PAGES_PAGES_H */