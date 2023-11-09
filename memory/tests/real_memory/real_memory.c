#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <instruction_memory/process/process.h>
#include <initial_configuration/memory_config.h>
#include <pcb/pcb.h>
#include <user_memory/real_memory.h>
#include <cspecs/cspec.h>

context (real_memory) {
    t_log* logger = log_create("./tests/tests.log", "TEST", false, LOG_LEVEL_INFO);
    t_config* config = config_create("./config/memory.config");
    
    describe("Testing Real Memory") {

        before {
            init_memory_config(config);
            init_real_memory();
            create_process(logger, crear_pcb(1, "1", 1, 1), 0);
        }end

        it("The frame table bitarray has the right amount of positions") {
            should_int(bitarray_get_max_bit(real_memory.frame_table)) be equal to(32);
        }end

        it("Testing real memory reading and writing") {
            write_on_frame(500, logger, 123);
            should_int((int) read_frame(500, logger)) be equal to(123);
        }end
    }end
}
