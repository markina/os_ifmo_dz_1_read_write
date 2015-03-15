#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

size_t read_(int fileDesc, void *buf, size_t count) {
	size_t retRead;
    int shift = 0;

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

size_t write_(int fileDesc, const void *buf, size_t count){
	int writen = 0;      
    size_t retWrite;
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
