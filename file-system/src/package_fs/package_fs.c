#include "package_fs.h"

char* deserialize_buffer(void* buffer) {
	int bytes;

	memcpy(&bytes, buffer, sizeof(int));

	char* info = malloc(bytes);

	memcpy(info, buffer + sizeof(int), bytes);

	return info;
}


