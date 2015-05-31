#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

extern ssize_t read_(int fileDesc, void *buf, size_t count);
extern ssize_t write_(int fileDesc, const void *buf, size_t count);
extern ssize_t read_until(int fileDesc, void * buf, size_t count, char delimiter);
extern int spawn(const char * file, char * const argv []);

typedef struct execargs_t
{
	int argc;
	char ** argv;
} execargs_t;

extern execargs_t *execargs_new(int argc);
extern void execargs_free(execargs_t *execargs);
extern int exec(execargs_t* execargs);
extern int runpiped(execargs_t** programs, size_t n);

#endif
