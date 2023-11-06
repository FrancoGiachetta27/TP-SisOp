#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <unistd.h>
#include <config/config.h>


void create_fat_file();
u_int32_t find_free_block();
void leer_fat();
void create_block_file();

#endif /* BITMAP_H_ */