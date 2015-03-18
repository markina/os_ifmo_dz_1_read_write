#include "../lib/helpers.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE_BUF 8192
char DELIMITER = ' ';

int main(int argc, char *argv[]) {
    
    char * buf = malloc(SIZE_BUF * sizeof( char)); 
    ssize_t readBytes;
    ssize_t reversed = 0;
    ssize_t lost = 0;
    while((readBytes = read_until(STDIN_FILENO, buf + lost * sizeof(char), SIZE_BUF - lost, DELIMITER)) > 0) {

    	for (ssize_t i = 0; i < readBytes + lost; i++) {
            
            if(buf[i] == DELIMITER) {
                for(ssize_t j = reversed; j < (i+reversed)/2; j++) {
                    char t = buf[i - 1 - (j - reversed)];
                    buf[i - 1 - (j - reversed)] = buf[j];
                    buf[j] = t;
                }
                
                if(write_(STDOUT_FILENO, buf + (reversed * sizeof(char)), i - reversed + 1) == -1) {
                    perror("Error while writing");
                }
                reversed = i+1;
            }
        }

        for(ssize_t i = 0, j = reversed; j < SIZE_BUF && j != 0; i++, j++) {
            buf[i] = buf[j];
        }
        
    	lost = readBytes - reversed;
        reversed = 0;
    }
   
    if(readBytes == 0) {
        for(ssize_t j = 0, i = lost - 1; j < lost/2; j++, i--) {
            char t = buf[j];
            buf[j] = buf[i];
            buf[i] = t;
        }
        
        if(write_(STDOUT_FILENO, buf, lost) == -1) {
            perror("Error while writing");
        }
        
    }

    if(readBytes == -1) {
    	perror("Error while reading");
    }

    free(buf);
	return 0;
}
