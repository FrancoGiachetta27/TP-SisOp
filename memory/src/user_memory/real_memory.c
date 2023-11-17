#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
t_user_space real_memory;

static void load_page_in_free_space(t_page_entry* page_referenced, int free_frame, int fs_socket, t_log* logger) {
    //swap_in(page_referenced, free_frame, fs_socket, logger);
    
    bitarray_set_bit(real_memory.frame_table, free_frame);
}

static void page_replace(t_page_entry* page_referenced, int fs_socket, t_log* logger) {
    pthread_mutex_lock(&mtx_select_page);
    t_page_entry* victim = list_get(pages_to_replace, 0);
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
    t_page_entry* page_referenced = reference_page(pid, page_number, logger);

    result.available 
        ? load_page_in_free_space(page_referenced, result.frame_number, fs_socket, logger)
        : page_replace(page_referenced, fs_socket, logger);
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