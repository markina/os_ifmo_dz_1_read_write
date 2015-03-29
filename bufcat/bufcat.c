#include "../lib/helpers.h"
#include "../lib/bufio.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE_BUF 8192
//#define SIZE_BUF 8

//TODO define DEBUG 
//TODO (echo -ne "abc def\ngh") | ./bufcat
//TODO ./cat < cat.c > cat2.c && diff cat.c cat2.c && echo OK


int main(int argc, char *argv[]) {
    
    buf_t * buf = buf_new(SIZE_BUF);
    while(buf_fill(STDIN_FILENO, buf, 1) != -1) {
        // printf("size = %i\n", (int)buf_size(buf));
        // printf("capacity = %i\n", (int)buf_capacity(buf));
        if(buf_flush(STDOUT_FILENO, buf, 1) == -1) {
            perror("Error while flushing:");        
        }
    }
    perror("Error while filling:");
    buf_free(buf);

	return 0;  
}
