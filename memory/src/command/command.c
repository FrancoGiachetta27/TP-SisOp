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
    while(op_code)
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
        case LOAD_PAGE:
            t_pag* received_page = receive_page(thread_info->port, thread_info->logger);
            load_page(received_page->pid, received_page->page_number, thread_info->logger);
            t_package* result_package = create_integer_package(LOAD_PAGE, 0);
            send_package(result_package, thread_info->port, thread_info->logger);
            break;
        case PAGE_NUMBER:
            t_pag* received_page2 = receive_page(thread_info->port, thread_info->logger);
            t_page* page1 = reference_page(received_page2->pid, received_page2->page_number, thread_info->logger);
            if (page1 == NULL) {
                send_page_fault(thread_info->port, thread_info->logger);
            } else {
                page1->bit_precense 
                    ? send_page_frame(page1, thread_info->port, thread_info->logger)
                    : send_page_fault(thread_info->port, thread_info->logger);
            }
            break;
        case FETCH_INSTRUCTION:
            t_pcb* pcb2 = receive_pcb(thread_info->port, thread_info->logger);
            char* next_instruction = fetch_next_instruction(pcb2->pid, pcb2->programCounter, thread_info->logger);
            log_debug(thread_info->logger, "Next instruction: %s", next_instruction);
            t_package* package_instruct = create_string_package(FETCH_INSTRUCTION, next_instruction);
            usleep(memory_config.time_delay * 1000);
            send_package(package_instruct, thread_info->port, thread_info->logger);
            destroy_pcb(pcb2);
            break;
        case MOV_OUT:
            t_mov_out* mov_out_page = receive_page_for_mov_out(thread_info->port, thread_info->logger);
            t_page* page2 = reference_page(mov_out_page->pid, mov_out_page->page_number, thread_info->logger);
            write_on_frame(mov_out_page->pid, page2->frame_number * memory_config.page_size + mov_out_page->displacement, thread_info->logger, mov_out_page->register_value);
            t_package* result_package1 = create_integer_package(MOV_OUT, 0);
            send_package(result_package1, thread_info->port, thread_info->logger);
            break;
        case MOV_IN:
            t_pag* mov_in_page = receive_page(thread_info->port, thread_info->logger);
            t_page* page3 = reference_page(mov_in_page->pid, mov_in_page->page_number, thread_info->logger);
            uint32_t value_in_frame = read_frame(mov_in_page->pid, page3->frame_number * memory_config.page_size + mov_in_page->displacement, thread_info->logger);
            t_package* result_package2 = create_uint32_package(MOV_OUT, value_in_frame);
            send_package(result_package2, thread_info->port, thread_info->logger);
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
        op_code = receive_op_code(thread_info->port, thread_info->logger);
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