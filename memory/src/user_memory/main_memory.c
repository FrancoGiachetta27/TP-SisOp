#include <user_memory/main_memory.h>

user_space main_memory;

void init_main_memory(void) {
    void* user_space = malloc(memory_config.memory_size);
    
    int total_frames = memory_config.memory_size/memory_config.page_size;
    void* frames = malloc(total_frames / 8);
    t_bitarray* frame_table = bitarray_create_with_mode(frames,total_frames / 8, LSB_FIRST);

    main_memory.frames = frames;
    main_memory.frame_table = frame_table;
}

int check_available_frames(void) {
    off_t i = 0;
    while(!bitarray_test_bit(main_memory.frame_table, i)) 
        i++;
    
    bitarray_set_bit(main_memory.frame_table, i);

    return i;
}

int* get_swap_blocks(int bytes, int socket_fs, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    // t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    // send_package(package, socket_fs, logger);

    // ... = receive_swap_blocks(socket_fs); 
    return 0;
}