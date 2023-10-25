#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <instruction_memory/process/process.h>
#include <initial_configuration/memory_config.h>
#include <cspecs/cspec.h>

context(process) {
    describe("Testing Process Actions") {
        t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
        t_config* config = config_create("./config/memory.config");
        char* test_instructions1[17] = {
            "SET AX 1",
            "SET BX 1",
            "SUM AX BX",
            "SUB AX BX",
            "MOV_IN DX 0",
            "MOV_OUT 0 CX",
            "WAIT RA",
            "SLEEP 10",
            "JNZ AX 4",
            "SIGNAL RA",
            "F_OPEN ARCHIVO W",
            "F_TRUNCATE ARCHIVO 64",
            "F_SEEK ARCHIVO 8",
            "F_WRITE ARCHIVO 0",
            "F_READ ARCHIVO 0",
            "F_CLOSE ARCHIVO",
            "EXIT"
        };

        char* test_instructions_sin_jnz[15] = {
            "SET AX 1",
            "SET BX 1",
            "SUM AX BX",
            "SUB AX BX",
            "MOV_IN DX 0",
            "MOV_OUT 0 CX",
            "WAIT RA",
            "SIGNAL RA",
            "F_OPEN ARCHIVO W",
            "F_TRUNCATE ARCHIVO 64",
            "F_SEEK ARCHIVO 8",
            "F_WRITE ARCHIVO 0",
            "F_READ ARCHIVO 0",
            "F_CLOSE ARCHIVO",
            "EXIT"
        };
        
        char* test_instructions_sin_recursos[13] = {
            "SET AX 1",
            "SET BX 1",
            "SUM AX BX",
            "SUB AX BX",
            "MOV_IN DX 0",
            "MOV_OUT 0 CX",
            "F_OPEN ARCHIVO W",
            "F_TRUNCATE ARCHIVO 64",
            "F_SEEK ARCHIVO 8",
            "F_WRITE ARCHIVO 0",
            "F_READ ARCHIVO 0",
            "F_CLOSE ARCHIVO",
            "EXIT"
        };

        before {
            init_memory_config(config);
            create_process(logger, 1, "1", 1, 0);
            create_process(logger, 2, "2", 1, 0);
            create_process(logger, 3, "3", 1, 0);
            create_process(logger, 4, "sin-recursos", 1, 0);
            create_process(logger, 5, "sin-jnz", 1, 0);
        }end

        it("El proceso se creo con 1.txt") {
            t_process* current_process = (t_process *)list_get(active_processes, 0);

            should_int(current_process->pid) be equal to(1);
            should_string(current_process->file_name) be equal to("1");
            should_int(current_process->bytes) be equal to(1);

            t_list* instruct_list = current_process->instructions_set;

            for (int i = 0; i < list_size(instruct_list); i++) {
                char* instr = list_get(instruct_list, i);

                should_string(instr) be equal to(test_instructions1[i]);
            }
        }end

        it("El proceso se creo con sin-recursos.txt") {
            t_process* current_process = (t_process *)list_get(active_processes, 3);

            should_int(current_process->pid) be equal to(4);
            should_string(current_process->file_name) be equal to("sin-recursos");
            should_int(current_process->bytes) be equal to(1);

            t_list* instruct_list = current_process->instructions_set;

            for (int i = 0; i < list_size(instruct_list); i++) {
                char* instr = list_get(instruct_list, i);

                should_string(instr) be equal to(test_instructions_sin_recursos[i]);
            }
        }end

        it("El proceso se creo con sin-jnz.txt") {
            t_process* current_process = (t_process *)list_get(active_processes, 4);

            should_int(current_process->pid) be equal to(5);
            should_string(current_process->file_name) be equal to("sin-jnz");
            should_int(current_process->bytes) be equal to(1);

            t_list* instruct_list = current_process->instructions_set;

            for (int i = 0; i < list_size(instruct_list); i++) {
                char* instr = list_get(instruct_list, i);

                should_string(instr) be equal to(test_instructions_sin_jnz[i]);
            }
        }end
    }end
    
}