#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <instruction_memory/process/process.h>
#include <initial_configuration/memory_config.h>
#include <user_memory/main_memory.h>
#include <cspecs/cspec.h>

context(mainMemory) {
    describe("Testing Process creation") {
        t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
        t_config* config = config_create("./config/memory.config");

        before {
            init_memory(config, &memory_config, &active_processes);
            init_main_memory();
            create_process(logger, 1, "1", 1, 0);
        }end

        it("the frame table bitarray has right amount of positions") {
            should_int(bitarray_get_max_bit(main_memory.frame_table)) be equal to(256);
        }end
    }end
}