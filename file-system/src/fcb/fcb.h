#ifndef FCB_H_INCLUDED
#define FCB_H_INCLUDED

#include <string.h>
#include <config/config.h>
#include "initial_configuration/fs_config.h"

void create_fcb_file(char* fileName);
void print_fcb_list();
t_fcb *find_fcb_file(char *file_name);
void update_fcb(t_fcb *fcb);

#endif