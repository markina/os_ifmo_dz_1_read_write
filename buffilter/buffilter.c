#include "../lib/helpers.h"
#include "../lib/bufio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE_BUF 8192

char DELIMITER = '\n';

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        printf("USAGE: ./buffilter [PATH] [OPTION]...\n");
        exit(0);
    }
    char nameProgram[SIZE_BUF];
    memcpy(nameProgram, argv[1], strlen(argv[1])*sizeof(char));
    nameProgram[strlen(argv[1])] = '\0';

    for(int i = 0; i < argc - 1; i++) {
        argv[i] = argv[i+1];
    }

    char addition[SIZE_BUF]; 
    argv[argc - 1] = addition;
    argv[argc] = NULL; 

    buf_t * buf = buf_new(SIZE_BUF);
    buf_t * bufOut = buf_new(SIZE_BUF);
    ssize_t retGetLine;
    while((retGetLine = buf_getline(STDIN_FILENO, buf, addition)) > 0) {
        retGetLine -= 1;
        addition[retGetLine] = '\0';
        int res = spawn(nameProgram, argv);
        addition[retGetLine] = '\n';
        if(res == 0) {

            if(buf_write(STDOUT_FILENO, bufOut, addition, retGetLine + 1) == -1) {
                buf_free(buf);
                perror("Error while writing");
            }
        }  
    }
    if(retGetLine == -1) {
        buf_free(buf);
        perror("Error while reading");
    }
    buf_flush(STDOUT_FILENO, bufOut, buf_size(buf));
    buf_free(buf);
    return 0;
}
