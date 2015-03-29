#ifndef _BUFIO_H_
#define _BUFIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct buf_t
{
	char * buffer;
	size_t capacity;
	size_t size;	
} buf_t;

extern buf_t *buf_new(size_t capacity);
extern void buf_free(buf_t *buf);
extern size_t buf_capacity(buf_t *buf);
extern size_t buf_size(buf_t *buf);

extern ssize_t buf_fill(int fileDesc, buf_t *buf, size_t required);
extern ssize_t buf_flush(int fileDesc, buf_t *buf, size_t required);


#endif
