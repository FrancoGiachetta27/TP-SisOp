#include <user_memory/main_memory.h>

int check_available_frames(void) {
    off_t i = 0;
    while(!bitarray_test_bit(main_memory.frame_table, i)) 
        i++;
    
    bitarray_set_bit(main_memory.frame_table, i);

    return i;
}

int get_swap_blocks(int bytes, int socket_fs, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    // t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    // send_package(package, socket_fs, logger);

    // ... = receive_swap_blocks(socket_fs); 
    return 0;
}
