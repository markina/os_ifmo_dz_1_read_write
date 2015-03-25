#include "../lib/helpers.h"
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
		printf("USAGE: ./filter [PATH] [OPTION]...\n");
        exit(0);
	}
	char nameProgram[SIZE_BUF];
	memcpy(nameProgram, argv[1], strlen(argv[1])*sizeof(char));
	nameProgram[strlen(argv[1])] = '\0';

	for(int i = 0; i < argc - 1; i++) {
		strcpy(argv[i], argv[i + 1]);
	}

	char addition[SIZE_BUF]; 
	argv[argc - 1] = addition;
	argv[argc] = NULL; 

	char buf[SIZE_BUF]; 
    ssize_t readBytes;
    ssize_t shift = 0;
    while((readBytes = read_until(STDIN_FILENO, buf + (shift * sizeof(char)), SIZE_BUF - shift, DELIMITER)) > 0) {
        ssize_t firstLetterInCurrentWord = 0;
        for (ssize_t i = 0; i < readBytes + shift; i++) {
            
            if(buf[i] == DELIMITER) {

            	if(i - firstLetterInCurrentWord == 0) {
            		firstLetterInCurrentWord = i+1;
            		continue;
            	}

            	memcpy(addition, buf + firstLetterInCurrentWord, (i - firstLetterInCurrentWord + 1)*sizeof(char));
            	addition[i - firstLetterInCurrentWord] = '\0';
				
				int res = spawn(nameProgram, argv);
				if(res == 0) {
	                if(write_(STDOUT_FILENO, buf + firstLetterInCurrentWord, i - firstLetterInCurrentWord + 1) == -1) {
			            perror("Error while writing");
			        }
			    }

                firstLetterInCurrentWord = i+1;
            }


        }

        memmove(buf, buf + firstLetterInCurrentWord, readBytes + shift - firstLetterInCurrentWord);
        shift += readBytes - firstLetterInCurrentWord;
    }
   
    if(readBytes == 0) {
        memcpy(addition, buf, shift*sizeof(char));
        addition[shift - 1] = '\0';
		
		int res = spawn(nameProgram, argv);
		if(res == 0) {
            if(write_(STDOUT_FILENO, buf, shift) == -1) {
	            perror("Error while writing");
	        }
	    }
    }

    if(readBytes == -1) {
        perror("Error while reading");
    }

    return 0;
}
