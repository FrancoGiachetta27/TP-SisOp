#include <command/command.h>

void* wait_for_command(t_thread *thread_info)
{
    int op_code = receive_op_code(thread_info->port, thread_info->logger);
    if (op_code == -1)
    {
        dictionary_remove(thread_info->dict, thread_info->dict_key);
        free(thread_info);
        return NULL;
    };
    while (op_code)
    {
        switch (op_code)
        {
        case ECHO_MEMORY:
            char *message = receive_buffer(thread_info->port, thread_info->logger);
            log_info(thread_info->logger, "OpCode: %d and Message: %s", op_code, message);
            free(message);
            break;
        case CREATE_PROCESS:
            t_pcb* pcb1 = receive_pcb(thread_info->port, thread_info->logger);
            int swap_blocks = get_swap_blocks(pcb1->tamanio, thread_info->conn->socket_filesystem, thread_info->logger);
            int is_ok = create_process(thread_info->logger, pcb1, swap_blocks);
            t_package* package_process = create_integer_package(PROCESS_OK, is_ok);
            send_package(package_process, thread_info->port, thread_info->logger);
            break;
        case FETCH_INSTRUCTION:
            t_pcb* pcb2 = receive_pcb(thread_info->port, thread_info->logger);
            char* next_instruction = fetch_next_instruction(pcb2->pid, pcb2->programCounter, thread_info->logger);
            executing_process = search_process(pcb2->pid);
            t_package* package_instruct = create_string_package(FETCH_INSTRUCTION, next_instruction);
            usleep(memory_config.time_delay * 1000);
            send_package(package_instruct, thread_info->port, thread_info->logger);
            destroy_pcb(pcb2);
            break;
        case PAGE_NUMBER:
            int page_number = *(int*) receive_buffer(thread_info->port, thread_info->logger);
            t_page* page = reference_page(page_number, thread_info->logger);
            if(page->bit_precense) send_page_frame(page, thread_info->port, thread_info->logger);
            else {
                send_page_fault(thread_info->port, thread_info->logger);
            }
            break;
        case END_PROCESS:
            t_pcb* pcb3 = receive_pcb(thread_info->port, thread_info->logger);
            deallocate_porcess(pcb3->pid);
            break;
        default:
            log_error(thread_info->logger, "Unknown OpCode");
            dictionary_remove(thread_info->dict, thread_info->dict_key);
            free(thread_info);
            return NULL;
        }
        int op_code = receive_op_code(thread_info->port, thread_info->logger);
        if (op_code == -1)
        {
            dictionary_remove(thread_info->dict, thread_info->dict_key);
            free(thread_info);
            return NULL;
        };
    }
    dictionary_remove(thread_info->dict, thread_info->dict_key);
    free(thread_info);
    return NULL;
}

void wait_in_every_port(t_conn *conn, t_log *logger)
{
    t_dictionary *dict = dictionary_create();
    for (int i = 0; i < MODULOS_A_CONECTAR; i++)
    {
        pthread_t thread_id;
        t_thread *thread_info = malloc(sizeof(t_thread));
        switch (i)
        {
        case 0:
            thread_info->dict_key = "FS";
            log_trace(logger, "Iniciada thread de FileSystem");
            thread_info->port = conn->socket_filesystem;
            break;
        case 1:
            thread_info->dict_key = "KRL";
            log_trace(logger, "Iniciada thread de Kernel");
            thread_info->port = conn->socket_kernel;
            break;
        case 2:
            thread_info->dict_key = "CPU";
            log_trace(logger, "Iniciada thread de CPU");
            thread_info->port = conn->socket_cpu;
            break;
        default:
            break;
        }
        thread_info->logger = logger;
        thread_info->dict = dict;
        thread_info->conn = conn;
        dictionary_put(dict, thread_info->dict_key, thread_info->dict_key);
        pthread_create(&thread_id, NULL, (void*) wait_for_command, thread_info);
        pthread_detach(thread_id);
    }
    while (!dictionary_is_empty(dict));
    dictionary_destroy(dict);
}