#ifndef SRC_ECHO_ECHO_H_
#define SRC_ECHO_ECHO_H_

#include <commons/log.h>
#include <package/package.h>
#include <initial_configuration/client_start.h>

enum MODULES_TO_ECHO {
	FS = 1,
	INTERRUPT = 2,
    DISPATCH = 3,
    MMY = 4
};

void echo(int modulo, t_conn* conn, t_log* logger);


#endif /* SRC_ECHO_ECHO_H_ */
