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

int getCountProg(char *line, size_t lenLine) { 
    int cntPipe = 0;
    for(int i = 0; i < lenLine; i++) {
        if(line[i] == '|') {
            cntPipe++;
        }
    }
    return cntPipe + 1;
}

int getCountWords(char * line, size_t begin, size_t end) {
    int count_words = 0;
    int isLeadingSpace = 1;
    for(size_t i = begin; i < end; i++) {
        if(line[i] == ' ') {
            while(i + 1 < end && line[i + 1] == ' ') {
                i++;
            }
            if(isLeadingSpace != 1) { 
                count_words++;
            }
        } else {
            isLeadingSpace = 0;
        }
    }
    if(end - 1 > 0 && line[end - 1] != ' ') {
        count_words++;
    }
    return count_words;
}

execargs_t * parseLine(char * line, size_t begin, size_t end) {
    int count_words = getCountWords(line, begin, end);
    execargs_t * execargs = execargs_new(count_words + 1);
    
    int isLeadingSpace = 1;
    int current_arg = 0;
    int current_lenword = 0;
    for(size_t i = begin; i < end; i++) {
        if(line[i] == ' ') {
            if(isLeadingSpace != 1) { 
                execargs->argv[current_arg] = (char*) malloc((current_lenword + 1) * sizeof(char));
                //memcpy(execargs->argv[current_arg], line + i - current_lenword, current_lenword);
                size_t k = 0;
                for(size_t j = i - current_lenword; j < i; j++) {
                    execargs->argv[current_arg][k] = line[j];
                    k++;
                }
                execargs->argv[current_arg][current_lenword] = 0;
                current_arg++;
                current_lenword = 0;
            }
            while(i + 1 < end && line[i + 1] == ' ') {
                i++;
            }
        } else {
            isLeadingSpace = 0;
            current_lenword++;
        }
    }
    if(end - 1 > 0 && line[end - 1] != ' ') {
        execargs->argv[current_arg] = (char*) malloc(current_lenword * sizeof(char));
        //memcpy(execargs->argv[current_arg], line + i - current_lenword, current_lenword);
        size_t k = 0;
        for(int j = end - current_lenword; j < end; j++) {
            execargs->argv[current_arg][k] = line[j];
            k++;
        }
        execargs->argv[current_arg][current_lenword] = 0;
        current_arg++;
        current_lenword = 0;
    }
    execargs->argv[current_arg] = NULL;
    // for(int i = 0; i < execargs->argc; i++) {
    //     printf("parseLine: execargs->argv[%d] = '%s'\n", i, execargs->argv[i]);
    // }
    return execargs;
}

void runRequest(char * line, size_t lenLine) {

    int countProg = getCountProg(line, lenLine);

    execargs_t ** execargses = malloc(countProg * sizeof(execargs_t));
    size_t begin = 0;
    size_t end = 0;
    int indexCurrentExecargs = 0;
    for(size_t i = 0; i < lenLine; i++) {
        if(line[i] == '|' || i == lenLine - 1) {
            if(i - 1 < 0) {
                printf("USEGE: ./file args | ./file args \\n \n");
                exit(1);
            }
            end = i;
            execargses[indexCurrentExecargs] = parseLine(line, begin, end);
            indexCurrentExecargs++;
            begin = i + 1;
        }
    }
    int retRunpiped = runpiped(execargses, countProg);

    for(int i = 0; i < countProg; i++) {
        execargs_free(execargses[i]);
    }
    free(execargses);

    if(retRunpiped == -1) {
        perror("Runpiped failed");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {

    // TODO ^C
    
    if(argc != 1) { 
        printf("USEGE: ./simplesh");
        return 0;
    }

    buf_t * buf = buf_new(SIZE_BUF);
    char line[SIZE_BUF];
    ssize_t retGetLine;
    write(STDOUT_FILENO, "$", 1);
    while((retGetLine = buf_getline(STDIN_FILENO, buf, line)) > 0) {
        //printf("$\n");
        runRequest(line, retGetLine);
        //printf("-----------------------------\n");
        write(STDOUT_FILENO, "$", 1);
    }
    buf_free(buf);
	return 0;
}
