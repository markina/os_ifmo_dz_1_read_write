#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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
