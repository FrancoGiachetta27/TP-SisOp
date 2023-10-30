// #include <commons/log.h>
// #include <commons/config.h>
// #include <commons/collections/list.h>
// #include <instruction_memory/process/process.h>
// #include <initial_configuration/memory_config.h>
// #include <pcb/pcb.h>
// #include <cspecs/cspec.h>

// context(process) {
//     describe("Testing Process") {
//         t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
//         t_config* config = config_create("./config/memory.config");

//         before {
//             init_memory_config(config);
//             create_process(logger, crear_pcb(1, "1", 1, 1), 0);
//             create_process(logger, crear_pcb(2, "2", 1, 1), 0);
//             create_process(logger, crear_pcb(3, "3", 1, 1), 0);
//             create_process(logger, crear_pcb(4, "sin-recursos", 1, 1), 0);
//             create_process(logger, crear_pcb(5, "sin-jnz", 1, 1), 0);
//         }end

//         it("Process created with 1.txt") {
//             t_process* current_process = (t_process *)list_get(active_processes, 0);

//             should_int(current_process->pid) be equal to(1);
//             should_string(current_process->file_name) be equal to("1");
//             should_int(current_process->bytes) be equal to(1);
//         }end
//         it("Process created with sin-recursos.txt") {
//             t_process* current_process = (t_process *)list_get(active_processes, 3);

//             should_int(current_process->pid) be equal to(4);
//             should_string(current_process->file_name) be equal to("sin-recursos");
//             should_int(current_process->bytes) be equal to(1);
//         }end
//         it("Process created with sin-jnz.txt") {
//             t_process* current_process = (t_process *)list_get(active_processes, 4);

//             should_int(current_process->pid) be equal to(5);
//             should_string(current_process->file_name) be equal to("sin-jnz");
//             should_int(current_process->bytes) be equal to(1);
//         }end
//         it("Processes are deallocated successfully") {
//             int pid;
//             int _is_pid(t_process* process) {
//                 return process->pid == pid;
//             };

//             pid = 2;
//             t_process* process = (t_process*)list_find(active_processes, (void*)_is_pid);
//             deallocate_porcess(process->pid);
//             should_ptr(list_find(active_processes, (void*)_is_pid)) be null;

//             pid = 3;
//             process = (t_process*)list_find(active_processes, (void*)_is_pid);
//             deallocate_porcess(process->pid);
//             should_ptr(list_find(active_processes, (void*)_is_pid)) be null;

//             pid = 4;
//             process = (t_process*)list_find(active_processes, (void*)_is_pid);
//             deallocate_porcess(process->pid);
//             should_ptr(list_find(active_processes, (void*)_is_pid)) be null;
//         }end
//     }end
// }