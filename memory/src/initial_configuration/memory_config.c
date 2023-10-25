#include <initial_configuration/memory_config.h>

t_memory_cfg memory_config;
user_space main_memory;
t_list* active_processes;

void init_memory_config(t_config* config) {
    active_processes = list_create(); // a list to keep track of every active process

    memory_config.memory_size = config_get_int_value(config, "TAM_MEMORIA");
    memory_config.page_size = config_get_int_value(config, "TAM_PAGINA");
    memory_config.instructions_path = config_get_string_value(config, "PATH_INSTRUCCIONES");
    memory_config.time_delay = config_get_int_value(config, "RETARDO_RESPUESTA");
    memory_config.algorithm = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
}

void init_main_memory(void) {
    void* user_space = malloc(memory_config.memory_size);
    
    int total_frames = memory_config.memory_size / memory_config.page_size;
    void* frames = malloc(total_frames / 8);
    t_bitarray* frame_table = bitarray_create_with_mode(frames,total_frames / 8, LSB_FIRST);
    free(frames);

    main_memory.frames = user_space;
    main_memory.frame_table = frame_table;

    bitarray_set_bit(main_memory.frame_table, 5);
}

void free_memory(void) {
    list_destroy_and_destroy_elements(active_processes, (void*) deallocate_porcess);
    free(main_memory.frames);
    bitarray_destroy(main_memory.frame_table);
}
