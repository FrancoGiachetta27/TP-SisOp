#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <unistd.h>
#include <config/config.h>

void create_fat_file(t_utils *utils);
int allocate_block();
void leer_fat();
void create_block_file(t_utils *utils);


#endif /* BITMAP_H_ */