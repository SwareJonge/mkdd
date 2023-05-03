#include "PowerPC_EABI_Support/MSL_C/MSL_Common/bsearch.h"

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *))
{
    int cmp;
    size_t lower, upper;
    size_t index;
    const void *p;

    if (key == NULL || base == NULL || nmemb == 0 || size == 0 || compar == NULL)
        return NULL;

    p = base;
    cmp = (*compar)(key, p);
    if(cmp == 0)
        return (void *)p;

    if(cmp < 0)
        return NULL;

    lower = 1;
    upper = nmemb - 1;
          
    while (lower <= upper)
    {
        index = (lower + upper) / 2;
        p = (const char *)base + (size * index);
        cmp = (*compar)(key, p);

        if (cmp == 0)
            return (void *)p;
        if (cmp < 0)
            upper = index - 1;
        else 
            lower = index + 1;
    }

    return NULL;
}