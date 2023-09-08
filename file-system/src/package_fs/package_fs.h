#ifndef SRC_INITIAL_CONFIGURATION_SERVER_START_H_
#define SRC_INITIAL_CONFIGURATION_SERVER_START_H_

#include <stdlib.h>
#include <string.h>

typedef enum {
	KERNEL_FILESYSTEM = 2,
	FILESYSTEM_MEMORY = 3,
	MEMORY_FILESYSTEM,
} op_code_filesystem;

char* deserialize_buffer(void* buffer);

#endif /* SRC_PACKAGE_FS_PACKAGE_FS_H_ */
