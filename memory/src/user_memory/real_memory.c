#include <user_memory/real_memory.h>

pthread_mutex_t mtx_frame_access;
t_user_space real_memory;

t_frame_search check_available_frames(void) {
    off_t i = 0;

    while(!bitarray_test_bit(real_memory.frame_table, i)) {
        if(i < bitarray_get_max_bit(real_memory.frame_table)) {
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

uint32_t read_frame(int real_address, t_log* logger) {
    uint32_t data;

    // if(real_address > memory_config.memory_size) hacer algo, no puede recibir una dirección mayor al tamaño;

    pthread_mutex_lock(&mtx_frame_access);
    memcpy(&data, real_memory.frames + real_address, sizeof(uint32_t));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: LEER - Direccion fisica: %d");
    
    return data;
}

void write_on_frame(int real_address, t_log* logger, uint32_t data) {

    // if(real_address > memory_config.memory_size) hacer algo, no puede recibir una dirección mayor al tamaño;

    pthread_mutex_lock(&mtx_frame_access);
    memcpy(real_memory.frames + real_address, &data, sizeof(uint32_t));
    pthread_mutex_unlock(&mtx_frame_access);

    log_info(logger, "PID: %d - Accion: ESCRIBIR - Direccion fisica: %d");
}

void load_page(int page_number, int pid) {
    t_frame_search result = check_available_frames();
    int frame_position;
    // void* page_data = get data from swap with the swap_pos
    
    if(result.available) {
        frame_position = result.frame_number * memory_config.page_size;
    }else { 
        t_page* victim = (t_page*) list_get(pages_to_replace, 0);
        
        frame_position = victim->frame_number * memory_config.page_size;

        if(victim->bit_modified == 1) {
            // ask fs to update the page in swap
        }
    }

    pthread_mutex_lock(&mtx_frame_access);
    // memcpy(real_memory.frames + frame_position, page_data, memory_config.page_size);
    pthread_mutex_unlock(&mtx_frame_access);

    t_page* page_referenced = search_on_table(pid, page_number); 
    page_referenced->bit_precense = 1;
}