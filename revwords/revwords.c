#include "../lib/helpers.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define SIZE_BUF 8192
char DELIMITER = ' ';

int main(int argc, char *argv[]) {
    
    char buf[SIZE_BUF]; 
    ssize_t readBytes;
    ssize_t shift = 0;
    while((readBytes = read_until(STDIN_FILENO, buf + (shift * sizeof(char)), SIZE_BUF - shift, DELIMITER)) > 0) {
        ssize_t firstLetterInCurrentWord = 0;
        for (ssize_t i = 0; i < readBytes + shift; i++) {
            
            if(buf[i] == DELIMITER) {
                for(ssize_t j = firstLetterInCurrentWord, x = i - 1; j < (i+firstLetterInCurrentWord)/2; x--, j++) {
                    char t = buf[x];
                    buf[x] = buf[j];
                    buf[j] = t;
                }
                firstLetterInCurrentWord = i+1;
            }
        }

        if(write_(STDOUT_FILENO, buf, firstLetterInCurrentWord) == -1) {
            perror("Error while writing");
        }

        memmove(buf, buf + firstLetterInCurrentWord, readBytes + shift - firstLetterInCurrentWord);
        
        shift += readBytes - firstLetterInCurrentWord;
    }
   
    if(readBytes == 0) {
        for(ssize_t j = 0, i = shift - 1; j < shift/2; j++, i--) {
            char t = buf[j];
            buf[j] = buf[i];
            buf[i] = t;
        }
        
        if(write_(STDOUT_FILENO, buf, shift) == -1) {
            perror("Error while writing");
        }
    }

    if(readBytes == -1) {
        perror("Error while reading");
    }

    
    return 0;
}
