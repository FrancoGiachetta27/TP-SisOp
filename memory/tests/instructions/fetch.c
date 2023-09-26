#include <commons/log.h>
//#include <commons/config.h>
#include <commons/collections/list.h>
#include <instruction_memory/instructions/instructions.h>
#include <instruction_memory/process/process.h>
#include <initial_configuration/memory_config.h>
#include <cspecs/cspec.h>

context(fetch)
{
    describe("Testing Fetch") {
        t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
        t_config* config = config_create("./config/memory.config");
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
        char* instruction;
        int i = 0;

        before {
            init_memory(config, &memory_config, &active_processes);
            create_process(logger, 1, "primero.txt", 0);
        }end

        it("Se obtuvieron todas las instrucciones con exito"){
            do {
                instruction = fetch_next_instruction(1, i);
                
                if(!string_is_empty(instruction)) should_string(instruction) be equal to(test_instructions[i]);
                
                i++;
            } while(!string_is_empty(instruction));
        }end
    }end
}