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

ssize_t getLenNumber(int n) {
    int cnt = 0;
    while(n > 0) {
        cnt++;
        n /= 10;
    }
    return cnt;
}

ssize_t itoa(int n, char * buf) {


    ssize_t lenNumber = getLenNumber(n);
    int i = lenNumber - 1;
    if(n == 0) {
        buf[0] = '0';
        return 1;
    }
    while(n > 0) {
        buf[i] = (n%10) + '0';
        n /= 10;
        i--;    
    }
    return lenNumber;
}

int main(int argc, char *argv[]) {

    char buf[SIZE_BUF]; 
    char bufout[SIZE_BUF * 2]; 

    ssize_t readBytes;
    ssize_t shift = 0;

    int lenCurrentWord = 0;
    ssize_t itBufout = 0;
    while((readBytes = read_until(STDIN_FILENO, buf + (shift * sizeof(char)), SIZE_BUF - shift, DELIMITER)) > 0) {
        ssize_t firstLetterInCurrentWord = 0;

        for (ssize_t i = shift; i < readBytes + shift; i++) {
            if(buf[i] == DELIMITER) {
                ssize_t lenNumber = itoa(lenCurrentWord, bufout + itBufout);
                itBufout += lenNumber;
                bufout[itBufout] = ' ';
                itBufout++;
                firstLetterInCurrentWord = i+1;
                lenCurrentWord = 0;
            } else {
                lenCurrentWord++;
            }
        }

        if(write_(STDOUT_FILENO, bufout, itBufout) == -1) {
            perror("Error while writing");
        }
        itBufout = 0;

        memmove(buf, buf + firstLetterInCurrentWord, readBytes + shift - firstLetterInCurrentWord);
        
        shift += readBytes - firstLetterInCurrentWord;
        
        lenCurrentWord = shift;
    }
   
    
    
    if(readBytes == 0) {
        char * strNumber;
        ssize_t lenNumber = itoa((int) lenCurrentWord, bufout);

        if(write_(STDOUT_FILENO, bufout, lenNumber) == -1) {
            perror("Error while writing");
        }
    }

    if(readBytes == -1) {
    	perror("Error while reading");
    }

	return 0;
}
