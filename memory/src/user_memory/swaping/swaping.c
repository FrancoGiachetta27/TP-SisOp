#include <user_memory/swaping/swaping.h>

static void* get_page_info(int swap_block, int fs_socket, t_log* logger) {
    t_package* package = create_integer_package(GET_FROM_SWAP, swap_block);
    send_package(package, fs_socket, logger);
    void* data = receive_buffer(fs_socket, logger);
    
    return data;
}

static void update_page(int fs_socket, t_page_entry* page, t_log* logger) {
    t_pag_swap* page_to_swap;
    int real_address = page->frame_number * memory_config.page_size;
    void* page_content = read_frame(real_address, memory_config.page_size);

    page_to_swap = page_for_swap(page->swap_position, memory_config.page_size, page_content);
    send_page_for_swap(UPDATE_SWAP, page_to_swap, fs_socket, logger);
    int* buffer = (int*) receive_buffer(fs_socket, logger);

    if(*buffer != 0) {
        log_error(logger, "Fail in response of page fault %d", *buffer);
        free(buffer);
    }

}

int get_swap_blocks(int bytes, int fs_socket, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    send_package(package, fs_socket, logger);

    //= receive_swap_blocks(fs_socket); 
    return 0;
}

void swap_in(t_page_entry* page_referenced, int frame_number, int fs_socket, t_log* logger) {
    void* page_data = get_page_info(page_referenced->swap_position, fs_socket, logger);
    int frame_position = frame_number * memory_config.page_size;

    page_referenced->frame_number = frame_number;
    page_referenced->bit_precense = 1;

    write_on_frame(frame_position, (size_t) memory_config.page_size, page_data);
    
    log_info(logger, "SWAP IN -  PID: %d - Marco: %d - Page In: %d-%d",
        page_referenced->pid,
        page_referenced->frame_number, 
        page_referenced->pid, 
        page_referenced->page_number
    );
}

void swap_out(t_page_entry* victim, int fs_socket, t_log* logger) {
    if(victim->bit_modified == 1) {
        //update_page(fs_socket, victim, logger);

        log_info(logger, "SWAP OUT -  PID: %d - Marco: %d - Page Out: %d-%d", 
            victim->pid, 
            victim->frame_number, 
            victim->pid, 
            victim->page_number
        );
    }

    victim->bit_precense = 0;
}