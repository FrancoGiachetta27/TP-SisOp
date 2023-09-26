#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <instruction_memory/process/process.h>
#include <initial_configuration/memory_config.h>
#include <cspecs/cspec.h>

context(createProcess) {
    describe("Testing Process") {
        t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
        t_config* config = config_create("./config/memory.config");
        int is_ok;
        char *test_instructions[17] = {
                "SET AX 1", 
                "SET BX 1", 
                "SUM AX BX", 
                "SUB AX BX", 
                "MOV_IN DX 0", 
                "MOV_OUT 0 CX", 
                "SLEEP 10", 
                "JNZ AX 4", 
                "WAIT RECURSO_1", 
                "SIGNAL RECURSO_1", 
                "F_OPEN ARCHIVO W", 
                "F_TRUNCATE ARCHIVO 64", 
                "F_SEEK ARCHIVO 8", 
                "F_WRITE ARCHIVO 0", 
                "F_READ ARCHIVO 0", 
                "F_CLOSE ARCHIVO", 
                "EXIT"
            };

        before {
            init_memory(config, &memory_config, &active_processes);
            is_ok = create_process(logger, 1, "primero.txt", 0);
        }end

        it("El proceso se creo con exito") {
            t_process *current_process = (t_process *)list_get(active_processes, 0);

            should_int(is_ok) be equal to(1);
            should_int(current_process->pid) be equal to(1);
            should_string(current_process->file_name) be equal to("primero.txt");
            should_int(current_process->bytes) be equal to(0);

            t_list* instruct_list = current_process->instructions_set;

            for (int i = 0; i < list_size(instruct_list); i++) {
                char* instr = list_get(instruct_list, i);

                should_string(instr) be equal to(test_instructions[i]);
            }
        }end
    }end
}