#include "types.h"
extern "C"{

void* __copy(char *dest, char *src, size_t size)
{
	char *p;
	
	if (dest && size) {
		p = dest;
		do {
			*p = *src;
			++p;
			++src;
			--size;
		} while (size);
	}
	
	return(dest);
}

void __init_arr(void)
{
	// UNUSED FUNCTION
}

void __new_arr(void)
{
	// UNUSED FUNCTION
}

void __del_arr(void)
{
	// UNUSED FUNCTION
}

void __dc_arr(void)
{
	// UNUSED FUNCTION
}

}
