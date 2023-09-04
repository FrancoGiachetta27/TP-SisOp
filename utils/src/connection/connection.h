#ifndef SRC_CONNECTION_CONNECTION_H_
#define SRC_CONNECTION_CONNECTION_H_

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/log.h>

int connect_to_server(char *ip, char* puerto, t_log* logger);
int start_server(char* port, t_log* logger);
int wait_for_client(int server_fd, t_log* logger);

#endif /* SRC_CONNECTION_CONNECTION_H_ */
