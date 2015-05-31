#include "../lib/helpers.h"
#include "../lib/bufio.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define SIZE_BUF 4000

int main(int argc, char *argv[]) {
    // if(argc != 1) { 
    //     printf("USEGE: ./simplesh");
    //     return 0;
    // }
    buf_t * buf = buf_new(SIZE_BUF);
    char line[SIZE_BUF];
    ssize_t retGetLine;
    while((retGetLine = buf_getline(STDIN_FILENO, buf, line)) > 0) {

        execargs_t * execargs = execargs_new(2);
        execargs->argv[0] = "cat";
        execargs->argv[0][0] = 't';
        execargs->argv[1] = "Makefile";
        execargs->argv[2] = NULL;
        exec(execargs);
        execargs_free(execargs);
        printf("-----------------------------\n");
    }
    buf_free(buf);

    //execargs_t * execargs = execargs_new(2);
    //execargs->argv[0] = argv[1];
    //execargs->argv[1] = argv[2];
    //exec(execargs);
    //execargs_free(execargs);
    //printf("-----------------------------\n");
	return 0;
}
