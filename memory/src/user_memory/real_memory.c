#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
t_user_space real_memory;

static void init_frame_table(void* user_space) {
    int total_frames = memory_config.memory_size / memory_config.page_size;
    void* frames = malloc(total_frames / 8);
    t_bitarray* frame_table = bitarray_create_with_mode(frames,total_frames / 8, LSB_FIRST);
    free(frames);

    real_memory.frames = user_space;
    real_memory.frame_table = frame_table;
}

static void load_page_in_free_space(t_page_entry* page_referenced, int free_frame, int fs_socket, t_log* logger) {
    swap_in(page_referenced, free_frame, fs_socket, logger);
    
    bitarray_set_bit(real_memory.frame_table, free_frame);
}

static void page_replace(t_page_entry* page_referenced, int fs_socket, t_log* logger) {
    pthread_mutex_lock(&mtx_select_page);
    t_page_entry* victim = list_get(pages_to_replace, 0);
    pthread_mutex_lock(&mtx_select_page);

    swap_out(victim, fs_socket, logger);
    swap_in(page_referenced, victim->frame_number, fs_socket, logger);

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
    t_page_entry* page_referenced = search_on_table(pid, page_number);

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

void init_real_memory(void) {
    void* user_space = malloc((uint32_t) memory_config.memory_size);
    page_tables = list_create();
    
    init_frame_table(user_space);
}

void free_memory(void) {
    list_destroy_and_destroy_elements(active_processes, (void*) deallocate_porcess);
    list_destroy_and_destroy_elements(pages_to_replace, (void*) destroy_page);
    list_destroy_and_destroy_elements(page_tables, (void*) destroy_page_table);
    free(real_memory.frames);
    bitarray_destroy(real_memory.frame_table);

    working = 0;
    sem_post(&sort_pages);
}