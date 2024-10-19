#ifndef RUNTIME_MEM_H
#define RUNTIME_MEM_H
#include "macros.h"
#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

DECL_SECTION(".init") void* memcpy(void* dest, const void* src, size_t n);
DECL_SECTION(".init") void __fill_mem(void* dest, int val, size_t count);
DECL_SECTION(".init") void* memset(void* dest, int val, size_t count);

#ifdef __cplusplus
}
#endif
#endif
