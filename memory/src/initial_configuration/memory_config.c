#include <initial_configuration/memory_config.h>

t_memory_cfg memory_config;

void init_memory(t_config* config, t_memory_cfg* memory_config, t_list** active_processes) {
    *active_processes = list_create(); // a list to keep track of every active process 

    memory_config->memory_size = config_get_int_value(config, "TAM_MEMORIA");
    memory_config->page_size = config_get_int_value(config, "TAM_PAGINA");
    memory_config->instructions_path = config_get_string_value(config, "PATH_INSTRUCCIONES");
    memory_config->time_delay = config_get_int_value(config, "RETARDO_RESPUESTA");
    memory_config->algorithm = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
}
