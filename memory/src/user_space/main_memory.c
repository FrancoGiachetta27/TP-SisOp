#include <user_space/main_memory.h>

void* space;
t_bitarray* frame_table;

int check_available_frames(void) {
    off_t i = 0;
    while(!bitarray_test_bit(frame_table, i)) i++;
    return i;
}

void get_swap_blocks(int bytes, int socket_fs, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    t_package* package_process = create_integer_package(GET_SWAP_BLOCKS, bytes);
    send_package(package_process, socket_fs, logger);

    // ... = receive_swap_blocks(socket_fs); 
    // return ...;
}