#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
t_user_space real_memory;

static void* swap_in(t_page* page_referenced, int frame_number, t_log* logger) {
    // void* page_data = ask swap for the page data
    int frame_position = frame_number * memory_config.page_size;

    page_referenced->frame_number = frame_number;
    page_referenced->bit_precense = 1;

    // pthread_mutex_lock(&mtx_frame_access);
    // memcpy(real_memory.frames + frame_position, page_data, memory_config.page_size);
    // pthread_mutex_unlock(&mtx_frame_access);
    
    log_info(logger, "SWAP IN -  PID: <PID> - Marco: <MARCO> - Page In: <PID>-<NRO_PAGINA>",
        page_referenced->pid,
        page_referenced->frame_number, 
        page_referenced->pid, 
        page_referenced->page_number);
}

static void swap_out(t_page* victim, t_log* logger) {
    if(victim->bit_modified == 1) {
        // ask fs to update the page in swap
        // log_info(logger, "SWAP OUT -  PID: <PID> - Marco: <MARCO> - Page Out: <PID>-<NRO_PAGINA>", 
        //     , 
        //     victim->frame_number, 
        //     victim->pid, 
        //     victim->page_number);
    }

    victim->bit_precense = 0;
}

static void load_page_in_free_space(t_page* page_referenced, int free_frame, t_log* logger) {
    swap_in(page_referenced, free_frame, logger);
    
    bitarray_set_bit(real_memory.frame_table, free_frame);
}

static void page_replace(t_page* page_referenced, t_log* logger) {
    pthread_mutex_lock(&mtx_select_page);
    t_page* victim = list_get(pages_to_replace, 0);
    pthread_mutex_lock(&mtx_select_page);

    swap_out(victim, logger);
    swap_in(page_referenced, victim->frame_number, logger);

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

    while(!bitarray_test_bit(real_memory.frame_table, i)) {
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

int get_swap_blocks(int bytes, int socket_fs, t_log* logger) {
    int total_blocks = bytes / memory_config.page_size;
    //t_package* package = create_integer_package(GET_SWAP_BLOCKS, total_blocks);
    //send_package(package, socket_fs, logger);

    // ... = receive_swap_blocks(socket_fs); 
    return 0;
}

uint32_t read_frame(int pid, int real_address, t_log* logger) {
    uint32_t data;

    pthread_mutex_lock(&mtx_frame_access);
    memcpy(&data, real_memory.frames + real_address, sizeof(uint32_t));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: LEER - Direccion fisica: %d", pid, real_address);
    
    return data;
}

void write_on_frame(int pid, int real_address, t_log* logger, uint32_t data) {
    pthread_mutex_lock(&mtx_frame_access);
    memcpy(real_memory.frames + real_address, &data, sizeof(uint32_t));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: ESCRIBIR - Direccion fisica: %d", pid, real_address);
}

void load_page(int pid, int page_number, t_log* logger) {
    t_frame_search result = check_available_frames();
    t_page* page_referenced = reference_page(pid, page_number, logger);

    if(result.available) load_page_in_free_space(page_referenced, result.frame_number, logger);
    else page_replace(page_referenced, logger);
}