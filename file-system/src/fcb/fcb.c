#include "fcb.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <config/config.h>
#include <commons/config.h>

// TODO: Agreguar funcionalidades para los FCBs

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result) {
        strcpy(result, s1);
        strcat(result, s2);
    }
    return result;
}

void create_fcb_file(t_utils* utils, char* file_name) {
    char *fcb_path = config_get_string_value(utils->config, "PATH_FCB");
    char *file_path = concat(concat(fcb_path, "/"), file_name);
    char *file_path_extesion = concat(file_path, ".fcb");

    int fd = open(file_path_extesion, O_CREAT | O_RDWR, S_IRWXU);

    if (fd != -1) {
        char* name = concat("NOMBRE_ARCHIVO=", file_name);
        char* size = "\nTAMANIO_ARCHIVO=0";
        char* initial_block = "\nBLOQUE_INICIAL=0";

        char* content = concat(name, size);
        content = concat(content, initial_block);

        size_t content_size = strlen(content);

        if (write(fd, content, content_size) == -1) {
            log_error(utils->logger, "No se pudo escribir en el archivo %s", file_name);
        }

        log_info(utils->logger, "Crear Archivo: %s", file_name);

        close(fd);
        free(content);
        free(name);
    } else {
        log_error(utils->logger, "No se pudo crear el archivo %s", file_name);
    }

    free(fcb_path);
    free(file_path);
    free(file_path_extesion);
}

