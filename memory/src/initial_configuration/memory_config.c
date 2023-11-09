#include <initial_configuration/memory_config.h>

t_memory_cfg memory_config;

static void init_frame_table(void* user_space) {
    int total_frames = memory_config.memory_size / memory_config.page_size;
    void* frames = malloc(total_frames / 8);
    t_bitarray* frame_table = bitarray_create_with_mode(frames,total_frames / 8, LSB_FIRST);
    free(frames);

    real_memory.frames = user_space;
    real_memory.frame_table = frame_table;
}

static void init_sorter_thread(void) {
    pthread_t page_replace_sorter_theard;
    pages_to_replace = list_create();
    sem_init(&sort_pages, 0, 0);
    
    if(string_equals_ignore_case(memory_config.algorithm, "FIFO")){
        pthread_create(&page_replace_sorter_theard, NULL, (void*) sort_pages_by_fifo, NULL);
    }else {
        pthread_create(&page_replace_sorter_theard, NULL, (void*) sort_pages_by_lru, NULL);
    }
    
    pthread_detach(page_replace_sorter_theard);
}

void init_memory_config(t_config* config) {
    active_processes = list_create(); // a list to keep track of every active process

    memory_config.memory_size = config_get_int_value(config, "TAM_MEMORIA");
    memory_config.page_size = config_get_int_value(config, "TAM_PAGINA");
    memory_config.instructions_path = config_get_string_value(config, "PATH_INSTRUCCIONES");
    memory_config.time_delay = config_get_int_value(config, "RETARDO_RESPUESTA");
    memory_config.algorithm = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
}

void init_real_memory(void) {
    void* user_space = malloc((uint32_t) memory_config.memory_size);
    page_tables = list_create();
    
    init_frame_table(user_space);
    init_sorter_thread();
}

void free_memory(void) {
    list_destroy_and_destroy_elements(active_processes, (void*) deallocate_porcess);
    free(real_memory.frames);
    bitarray_destroy(real_memory.frame_table);

    working = 0;
    sem_post(&sort_pages);
}