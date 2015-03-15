#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include <stdlib.h>

extern size_t read_(int fileDesc, void *buf, size_t count);
extern size_t write_(int fileDesc, const void *buf, size_t count);

#endif
