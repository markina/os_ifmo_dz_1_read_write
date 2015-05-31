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
        //printf(line);
        //printf("%d\n", strlen(line));
        int cnt_space = 0;
        for(int i = 0; i < retGetLine; i++) {
            if(line[i] == ' ') {
                cnt_space++;
            }
        }
        printf("cnt_space = %d\n", cnt_space);
        //write(STDOUT_FILENO, line, 3);
        execargs_t * execargs = execargs_new(2);
        //execargs->argv[0][0] = 'c';
        // execargs->argv[0][1] = 'a';
        // execargs->argv[0][2] = 't';
        // execargs->argv[0][3] = '\0';
        //memcpy(execargs->argv[0], line, 3);
        //memcpy(execargs->argv[1], line + 4, 8);
        //execargs->argv[2] = NULL;
        //execargs->argv[0] = argv[1];
        //execargs->argv[1] = argv[2];
        //printf(execargs->argv[0]);
        //printf(execargs->argv[1]);
        //printf(execargs->argv[3]);

        //exec(execargs);
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
