#include "fcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <config/config.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/memory.h>

// TODO: Agreguar funcionalidades para los FCBs

void create_fcb_file(t_utils* utils, char* file_name) {
    char* fcb_path = config_get_string_value(utils->config, "PATH_FCB");

    string_append(&fcb_path, "/");
    string_append(&fcb_path, file_name);
    string_append(&fcb_path, ".fcb");

    int fd = open(fcb_path, O_CREAT | O_RDWR, S_IRWXU);

    if (fd != -1) {

        char* name = string_duplicate("NOMBRE_ARCHIVO=");
        string_append(&name, file_name);
        char* size = "\nTAMANIO_ARCHIVO=0";
        char* initial_block = "\nBLOQUE_INICIAL=0";

        char* content = string_new();
        string_append(&content, name);
        string_append(&content, size);
        string_append(&content, initial_block);

        size_t content_size = string_length(content);

        ftruncate(fd, content_size);

        char* mapped_data = mmap(NULL, content_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (mapped_data == MAP_FAILED) {
            log_error(utils->logger, "No se pudo mapear el archivo %s", file_name);
        } else {
            memcpy(mapped_data, content, content_size);

            // Print valores
            // char* hex = mem_hexstring(mapped_data, content_size);
            // printf("HEX: %s\n", hex);

            msync(mapped_data, content_size, MS_SYNC);
            munmap(mapped_data, content_size);

            log_info(utils->logger, "Archivo creado: %s", file_name);
        }

        close(fd);
        free(content);
        free(name);
    } else {
        log_error(utils->logger, "No se pudo crear el archivo %s", file_name);
    }

    free(fcb_path); 
}