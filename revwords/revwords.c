#include "../lib/helpers.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE_BUF 4000
char DELIMITER = ' ';

int main(int argc, char *argv[]) {
    
    char * buf = malloc(SIZE_BUF * sizeof(char)); 
    ssize_t readBytes;
    while((readBytes = read_until(STDIN_FILENO, buf, SIZE_BUF * sizeof(char), DELIMITER)) > 0) {
    	for (ssize_t i = 0; i < readBytes; i++) {
            if(((char *)buf)[i] == DELIMITER) {
                for(ssize_t j = 0; j < i/2; j++) {
                    char t = buf[i - 1 - j];
                    buf[i - 1 - j] = buf[j];
                    buf[j] = t;
                }
                if(write(STDOUT_FILENO, buf, i) == -1) {
                    perror("Error while writing");
                } 
                continue;       
            }
        }
    	
    }

    if(readBytes == -1) {
    	perror("Error while reading");
    }
    free(buf);
	return 0;
}
