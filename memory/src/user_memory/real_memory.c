#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
user_space real_memory;

int check_available_frames(void) {
    off_t i = 0;
    while(!bitarray_test_bit(real_memory.frame_table, i)) 
        i++;
    
    bitarray_set_bit(real_memory.frame_table, i);

    return i;
}

int get_swap_blocks(int bytes, int socket_fs, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    // t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    // send_package(package, socket_fs, logger);

    // ... = receive_swap_blocks(socket_fs); 
    return 0;
}

int read_frame(int real_address, t_log* logger) {
    int data;

    pthread_mutex_lock(&mtx_frame_access);
    memcpy(&data, real_memory.frames + real_address, sizeof(int));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: LEER - Direccion fisica: %d");
    
    return data;
}

void write_on_frame(int real_address, t_log* logger, int data) {
    pthread_mutex_lock(&mtx_frame_access);
    memcpy(real_memory.frames + real_address, &data, sizeof(int));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: ESCRIBIR - Direccion fisica: %d");
}

void load_page(int log_address) {}