#include "../lib/helpers.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE_BUF 4000

int main(int argc, char *argv[]) {
    
    char buf[SIZE_BUF]; 
    ssize_t readBytes;
    while((readBytes = read_(STDIN_FILENO, buf, SIZE_BUF * sizeof(char))) > 0) {
    	
    	if(write_(STDOUT_FILENO, buf, readBytes) == -1) {
    		perror("Error while writing");
    	}
    }

    if(readBytes == -1) {
    	perror("Error while reading");
    }
    
	return 0;
}
