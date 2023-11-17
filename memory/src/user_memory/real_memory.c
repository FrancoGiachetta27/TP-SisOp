#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
t_user_space real_memory;

static void* get_page_info(int swap_block, int fs_socket, t_log* logger) {
    t_package* package = create_integer_package(GET_FROM_SWAP, swap_block);
    send_package(package, fs_socket, logger);
    void* data = receive_buffer(fs_socket, logger);
    
    return data;
}

static void update_page(int fs_socket, t_page* page, t_log* logger) {
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

static void swap_in(t_page* page_referenced, int frame_number, int fs_socket, t_log* logger) {
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

static void swap_out(t_page* victim, int fs_socket, t_log* logger) {
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

static void load_page_in_free_space(t_page* page_referenced, int free_frame, int fs_socket, t_log* logger) {
    //swap_in(page_referenced, free_frame, fs_socket, logger);
    
    bitarray_set_bit(real_memory.frame_table, free_frame);
}

static void page_replace(t_page* page_referenced, int fs_socket, t_log* logger) {
    pthread_mutex_lock(&mtx_select_page);
    t_page* victim = list_get(pages_to_replace, 0);
    pthread_mutex_lock(&mtx_select_page);

    swap_out(victim, fs_socket, logger);
    //swap_in(page_referenced, victim->frame_number, fs_socket, logger);

    log_info(logger, 
        "REEMPLAZO - Marco: %d - Page Out: %d-%d - Page In: %d-%d", 
        victim->frame_number, 
        victim->pid, 
        victim->page_number, 
        page_referenced->pid, 
        page_referenced->page_number
    );
}

t_frame_search check_available_frames(void) {
    off_t i = 0;

    while(bitarray_test_bit(real_memory.frame_table, i)) {
        if(i > bitarray_get_max_bit(real_memory.frame_table)) {
            t_frame_search result = { .available = false };
            return result;
        } 
        i++;
    }
    
    bitarray_set_bit(real_memory.frame_table, i);

    t_frame_search result = {.available = true, .frame_number = i};

    return result;
}

void load_page(int pid, int page_number, int fs_socket, t_log* logger) {
    t_frame_search result = check_available_frames();
    t_page* page_referenced = reference_page(pid, page_number, logger);

    result.available 
        ? load_page_in_free_space(page_referenced, result.frame_number, fs_socket, logger)
        : page_replace(page_referenced, fs_socket, logger);
}

int get_swap_blocks(int bytes, int fs_socket, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    send_package(package, fs_socket, logger);

    //= receive_swap_blocks(fs_socket); 
    return 0;
}

void* read_frame(int real_address, size_t size) {
    void* data = malloc(size);

    pthread_mutex_lock(&mtx_frame_access);
    memcpy(data, real_memory.frames + real_address, size);
    pthread_mutex_unlock(&mtx_frame_access);
    
    return data;
}

void write_on_frame(int real_address, size_t size, void* data) {
    pthread_mutex_lock(&mtx_frame_access);
    memcpy(real_memory.frames + real_address, data, size);
    pthread_mutex_unlock(&mtx_frame_access);
}