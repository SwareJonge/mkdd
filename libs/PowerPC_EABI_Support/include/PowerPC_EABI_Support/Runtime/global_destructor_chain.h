#ifndef _GLOBALDESTRUCTORCHAIN
#define _GLOBALDESTRUCTORCHAIN

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DestructorChain {
	struct DestructorChain* next;
	void* destructor;
	void* object;
} DestructorChain;

void* __register_global_object(void* object, void* destructor, void* registration);

void __destroy_global_chain(void);

int __register_atexit(void (*)(void));

#ifdef __cplusplus
}
#endif

#endif
