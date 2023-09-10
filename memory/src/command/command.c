#include <command/command.h>

void* wait_for_command(t_thread* thread_info) {
    int op_code = receive_op_code(thread_info->port, thread_info->logger);
	if (op_code == -1) {
        dictionary_remove(thread_info->dict, thread_info->dict_key);
        free(thread_info);
        return NULL;
    };
	while (op_code)
	{
		switch (op_code) {
			case ECHO_MEMORY:
				char* message = receive_buffer(thread_info->port, thread_info->logger);
				log_info(thread_info->logger, "OpCode: %d and Message: %s", op_code, message);
				free(message);
				break;
			default:
				log_error(thread_info->logger, "Unknown OpCode");
                dictionary_remove(thread_info->dict, thread_info->dict_key);
                free(thread_info);
				return NULL;
		}
		int op_code = receive_op_code(thread_info->port, thread_info->logger);
	    if (op_code == -1) {
            dictionary_remove(thread_info->dict, thread_info->dict_key);
            free(thread_info);
            return NULL;
        };
	}
    dictionary_remove(thread_info->dict, thread_info->dict_key);
    free(thread_info);
    return NULL;
}

void wait_in_every_port(t_conn* conn, t_log* logger) {
    t_dictionary* dict = dictionary_create();
	for (int i = 0; i < MODULOS_A_CONECTAR; i++) {
		pthread_t thread_id;
        t_thread* thread_info = malloc(sizeof(t_thread));
        switch (i) {
            case 0:
                thread_info->dict_key = "FS";
                log_trace(logger, "Iniciada thread de FileSystem");
                thread_info->port=conn->socket_filesystem;
                break;
            case 1:
                thread_info->dict_key = "KRL";
                log_trace(logger, "Iniciada thread de Kernel");
                thread_info->port=conn->socket_kernel;
                break;
            case 2:
                thread_info->dict_key = "CPU";
                log_trace(logger, "Iniciada thread de CPU");
                thread_info->port=conn->socket_cpu;
                break;
        default:
            break;
        }
        thread_info->logger = logger;
        thread_info->dict = dict;
		dictionary_put(dict, thread_info->dict_key, thread_info->dict_key);
		pthread_create(&thread_id, NULL, wait_for_command, thread_info);
		pthread_detach(thread_id);
	}
    while (!dictionary_is_empty(dict));
    dictionary_destroy(dict);
}