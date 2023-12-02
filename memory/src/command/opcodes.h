#ifndef SRC_COMMAND_OPCODES_H_
#define SRC_COMMAND_OPCODES_H_

enum KERNEL_CODE_OP
{
    CREATE_PROCESS = 2,
    PROCESS_OK = 5,
    LOAD_PAGE = 11,
    END_PROCESS = 6,
};

enum CPU_CODE_OP
{
    FETCH_INSTRUCTION = 3,
    PAGE_SIZE = 4,
    PAGE_NUMBER = 8,
    PAGE_FRAME = 9,
    PAGE_FAULT_COMMAND = 10,
    MOV_IN = 15,
    MOV_OUT = 16,
};

enum FS_OP
{
    GET_SWAP_BLOCKS = 7,
    GET_FROM_SWAP = 12,
    UPDATE_SWAP = 13,
    FREE_PAGES = 14,
    MOV_IN_FS = 17,
    MOV_OUT_FS = 18,
};

#endif /* SRC_COMMAND_OPCODES_H_ */