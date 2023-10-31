#ifndef FCB_H_INCLUDED
#define FCB_H_INCLUDED

#include <string.h>
#include <config/config.h>

void create_fcb_file(t_utils* utils, char* fileName);
t_config* create_fcb_config(t_utils* utils, char* file_name);


#endif