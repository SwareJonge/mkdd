#ifndef _MEM_H
#define _MEM_H

#include "types.h"
#include "PowerPC_EABI_Support/Runtime/__mem.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int memcmp(const void*, const void*, size_t);
void *__memrchr(const void *src, int val, size_t n);
void *memchr(const void *src, int val, size_t n);
void *memmove(void *, const void *, size_t);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
