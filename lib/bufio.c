#include "bufio.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

// void check(buf_t *buf) {
// 	if(buf == NULL || buf->buffer == NULL) {
// 		buf_free(buf);
// 		#ifdef DEBUG
// 		abort();
// 		#endif
// 	} 
// }

buf_t *buf_new(size_t capacity){
	buf_t * buf = malloc(sizeof(buf_t));
	if (buf == NULL) {
		return NULL;
	}

	buf->buffer = malloc(capacity * sizeof(char));
	if(buf->buffer == NULL) {
		free(buf);
		return NULL;
	}
	buf->capacity = capacity;
	buf->size = 0;
	return buf;
}

void buf_free(buf_t *buf){
	if(buf == NULL || buf->buffer == NULL) {
		if(buf != NULL) {
			free(buf);
		}
		return;
	}
	free(buf->buffer);
	free(buf);
}

size_t buf_capacity(buf_t *buf){
//	check(buf);
	return buf->capacity;
}

size_t buf_size(buf_t *buf){
//	check(buf);
	return buf->size;
}

ssize_t buf_fill(int fileDesc, buf_t *buf, size_t required){
//	check(buf);

	ssize_t retRead;
    while((retRead = read(fileDesc, buf->buffer + buf->size * sizeof(char), buf->capacity - buf->size)) > 0) {
    	buf->size += retRead;
    	if(buf->size >= required){
    		return buf->size;
    	}
    }
    if(retRead == -1) {
    	return -1;
    } else {
    	return buf->size;
    }
}

ssize_t buf_flush(int fileDesc, buf_t *buf, size_t required){
//	check(buf);

	ssize_t writen = 0;
	ssize_t retRead;
    while((retRead = write(fileDesc, buf->buffer + writen * sizeof(char), buf->size - writen)) > 0) {
		writen += retRead;
		if(writen >= required){
			memmove(buf->buffer, buf->buffer + writen, buf->capacity - writen);
			buf->size -= writen;
    		return buf->size;
    	}
    }
    if(retRead == -1) {
    	return -1;
    } else {
		memmove(buf->buffer, buf->buffer + writen, buf->capacity - writen);
		buf->size -= writen;
		return buf->size;
    }	
}

