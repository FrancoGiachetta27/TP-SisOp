#ifndef FCB_H_INCLUDED
#define FCB_H_INCLUDED

#include <string.h>
#include <config/config.h>

char* concat(const char *s1, const char *s2);
void create_fcb_file(t_utils* utils, char* fileName);

#endif