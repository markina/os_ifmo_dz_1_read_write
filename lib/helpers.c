#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

ssize_t read_(int fileDesc, void *buf, size_t count) {
	ssize_t retRead;
    ssize_t shift = 0;

    while((retRead = read(fileDesc, buf + shift * sizeof(char), count)) > 0) {
    	count -= retRead;
    	shift += retRead;
    }
    if(retRead == -1) {
    	return -1;
    } else {
    	return shift;
    }
}

ssize_t write_(int fileDesc, const void *buf, size_t count) {
	ssize_t writen = 0;      
    ssize_t retWrite;
    while(count > 0) {
        retWrite = write(fileDesc, buf +  writen, count);
        if(retWrite == -1) {
            return -1;
        }
        count -= retWrite; 
        writen += retWrite;
    }                 
    return writen;    
}

ssize_t read_until(int fileDesc, void * buf, size_t count, char delimiter) {
    ssize_t retRead;
    ssize_t shift = 0;

    while((retRead = read(fileDesc, buf + shift * sizeof(char), count)) > 0) {
        for (ssize_t i = shift; i < shift + retRead; i++) {
            if(((char *)buf)[i] == delimiter) {
                return shift + retRead;
            }
        }
        count -= retRead;
        shift += retRead;
    }
    if(retRead == -1) {
        return -1;
    } else {
        return shift;
    }        
}

int spawn(const char * file, char * const argv []) {

    pid_t retFork = fork();
    if(retFork == -1) {
        perror("Fork failed");
    }
    if(retFork) {
        int status;            
        pid_t retWait = waitpid (retFork , &status, 0);
        if(retWait == -1) {
            perror("Wait failed");
        }
        return status;
    } else {
        int retExec = execvp(file, argv);
        if(retExec == -1) {
            perror("Error while execing");
            exit(-1);
        }
    }
}

#define ARG_SIZE 256

execargs_t *execargs_new(int argc) {
    execargs_t *execargs = malloc(sizeof(execargs_t));
    if(execargs == NULL) {
        perror("Calloc failed");
        return NULL;
    }
    //printf("constructor allocated %i bytes", sizeof(execargs_t));
    //execargs_t *execargs;
    execargs->argc = argc;
    execargs->argv = (char **) calloc(argc + 1, sizeof(char*));
    if(execargs->argv == NULL) {
        free(execargs);
        perror("Calloc failed");
        return NULL;
    }

    for(int i = 0; i < execargs->argc; i++) {
        execargs->argv[i] = malloc(sizeof(char) * ARG_SIZE);
    }

    //printf("constructor:\n"); 
    // printf("argc: %i\n", argc);    
    // for (int i; i <= argc + 1; i++)
    // {
    //     printf("%i\n", execargs->argv[i]);
    // }
    // printf("constructor end\n");
    
    return execargs;
}

void execargs_free(execargs_t *execargs) {
    //for(int i = 0; i < execargs->argc; i++) {
        //free(execargs->argv[i]);
    //}
    free(execargs->argv);
    free(execargs);
}

int exec(execargs_t* execargs) {
    char * file = execargs->argv[0];

    //file[srtlen(execargs->argv[0])] = '\0';
    if(spawn(file, execargs->argv) == -1) {
        return -1;
    } else {
        return 0;
    }
}

int runpiped(execargs_t** programs, size_t n){
    for(int i = 0; i < n; i++) {
        pid_t retFork = fork();
        if(retFork == -1) {
            perror("Fork failed");
        }
        if(retFork) {
            int status;            
            pid_t retWait = waitpid (retFork , &status, 0);
            if(retWait == -1) {
                perror("Wait failed");
            }
            if(status != 0) {
                return status;
            }
        } else {
            int retExec = exec(programs[i]);
            if(retExec == -1) {
                perror("Error while execing");
                exit(-1);
            }
        }
    }

    return -1;
}
