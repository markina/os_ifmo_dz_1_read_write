#include "../lib/helpers.h"
#include "../lib/bufio.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE_BUF 8192

int main(int argc, char *argv[]) {
    
    buf_t * buf = buf_new(SIZE_BUF);
    ssize_t retFill;
    while(retFill = buf_fill(STDIN_FILENO, buf, 1) > 0) {
        if(buf_flush(STDOUT_FILENO, buf, 1) == -1) {
            perror("Error while flushing:");        
        }
    }
    if(retFill == -1) {
        perror("Error while filling:");
    }
    buf_free(buf);

	return 0;  
}
