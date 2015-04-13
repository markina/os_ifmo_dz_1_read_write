#include "bufio.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void check(buf_t *buf) {
	#ifdef DEBUG
	if(buf == NULL || buf->buffer == NULL) {
		buf_free(buf);
		abort();
	} 
	#endif
}

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
	check(buf);
	return buf->capacity;
}

size_t buf_size(buf_t *buf){
	check(buf);
	return buf->size;
}

ssize_t buf_fill(int fileDesc, buf_t *buf, size_t required){
	check(buf);

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
	check(buf);

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

ssize_t buf_getline(int fileDesc, buf_t *buf, char *dest){
	check(buf);

	if(buf->size != 0) {
		for(size_t i = 0; i < buf->size; i++) {
			if(buf->buffer[i] == '\n') {
				memcpy(dest, buf->buffer, i);
				memmove(buf->buffer, buf->buffer + i + 1, i);
				buf->size -= i + 1;
				return i + 1;
			}
		}
	}
	size_t firstNoCheckedIndex = buf->size;
	ssize_t oldRetFill = -1;
	ssize_t retFill;
	while((retFill = buf_fill(fileDesc, buf, 1)) > 0 && retFill != oldRetFill) {
		oldRetFill = retFill;
		for(; firstNoCheckedIndex < buf->size; firstNoCheckedIndex++) {
			if(buf->buffer[firstNoCheckedIndex] == '\n') {
				memcpy(dest, buf->buffer, firstNoCheckedIndex);
				if(buf->size > firstNoCheckedIndex + 1) {
					memmove(buf->buffer, buf->buffer + firstNoCheckedIndex + 1, firstNoCheckedIndex);
				}
				buf->size -= firstNoCheckedIndex + 1;
				return firstNoCheckedIndex + 1;
			}
		}
	}
	if(retFill == -1) {
		return -1;
	} else {
		if(buf->size == 0) {
			return 0;
		}
		memcpy(dest, buf->buffer, buf->size);
		size_t length = buf->size;
		buf->size = 0;
		return length;
	}
}

ssize_t buf_write(int fileDesc, buf_t *buf, char *src, size_t len){
	check(buf);

	while(len > 0) {
		if(buf->size + len <= buf->capacity) {
			memcpy(buf->buffer + buf->size, src, len);
			buf->size += len;
			return len;
		} else {
			memcpy(buf->buffer + buf->size, src, buf->capacity - buf->size);
			memmove(src, src + buf->capacity - buf->size, len - (buf->capacity - buf->size));
			len -= buf->capacity - buf->size;
			buf->size = buf->capacity;
			if(buf_flush(fileDesc, buf, 1) == -1) {
				return -1;
			}
		}
	}
}

