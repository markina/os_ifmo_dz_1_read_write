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
    //perror("exec: \n");
    char * file = execargs->argv[0];

    //file[srtlen(execargs->argv[0])] = '\0';
    if(spawn(file, execargs->argv) == -1) {
        return -1;
    } else {
        return 0;
    }
}

int runpiped(execargs_t** programs, size_t n){
    
    // printf("runpiped: pipefd[0] = %d\n", pipefd[0]);
    // printf("runpiped: pipefd[1] = %d\n", pipefd[1]);
    //int oldPipe = pipefd[1];
    for(int i = 0; i < n; i++) {

        int pipefd[2];
        if(pipe(pipefd) == -1) {
            perror("Pipe failed");
        }
        int oldPipe;

        pid_t retFork = fork();
        if(retFork == -1) {
            perror("Fork failed");
        }
        if(retFork) {
            close(pipefd[1]);
            int status;        
            //printf("We wait my child\n");    
            pid_t retWait = waitpid (retFork , &status, 0);
            if(retWait == -1) {
                perror("Wait failed");
            }
            if(status != 0) {
                return -1;
            }
            oldPipe = pipefd[0];
            if(i == n - 1) {
                close(oldPipe);
                return status;
            }
        } else {
            //printf("I'm child! :)\n");
            if(n == 1) {
                close(pipefd[0]);
                close(pipefd[1]);
                int retExec = exec(programs[i]);
                if(retExec == -1) {
                    perror("Error while execing");
                    exit(-1);
                }
                _exit(EXIT_SUCCESS);
            }
            if(i == 0) {
                // printf("first iteration\n");
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                int retExec = exec(programs[i]);
                if(retExec == -1) {
                    perror("Error while execing");
                    exit(-1);
                }
                close(pipefd[1]);
            } else if (i != n - 1) {
                //perror("iteration\n");
                //oldPipe = pipefd[0];
                //close(pipefd[1]);
                

                // if(pipe(pipefd) == -1) {
                //     perror("Pipe failed");
                //     exit(-1);
                // }
                // printf("runpiped: oldPipe = %d\n", oldPipe);
                // printf("runpiped: pipefd[0] = %d\n", pipefd[0]);
                // printf("runpiped: pipefd[1] = %d\n", pipefd[1]);    
                
                close(pipefd[0]);
                dup2(oldPipe, STDIN_FILENO);
                dup2(pipefd[1], STDOUT_FILENO);
                int retExec = exec(programs[i]);
                if(retExec == -1) {
                    perror("Error while execing");
                    exit(-1);
                }
                close(oldPipe);
                close(pipefd[1]);
                //close(oldPipe);

            } else {
                //perror("last iteration\n");
                //close(pipefd[1]);
                close(pipefd[0]);
                close(pipefd[1]);
                dup2(oldPipe, STDIN_FILENO);
                int retExec = exec(programs[i]);
                if(retExec == -1) {
                    perror("Error while execing");
                    exit(-1);
                }
                close(oldPipe);
                dup2(STDIN_FILENO, STDIN_FILENO);
                dup2(STDOUT_FILENO, STDOUT_FILENO);
            }
            _exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
