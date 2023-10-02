#include <instructions/check_interrupt.h>

int check_interrupt(int interrupt_fd, t_log* logger) {
    int bytes_read = 0;
    ioctl(interrupt_fd,FIONREAD,&bytes_read); 
    switch (bytes_read)
    {
        case FAIL_CONNECTION:
            return FAIL_CONNECTION;
        case NO_INTERRUPTION:
            return NO_INTERRUPTION;
    }
    return INTERRUPTION;
}