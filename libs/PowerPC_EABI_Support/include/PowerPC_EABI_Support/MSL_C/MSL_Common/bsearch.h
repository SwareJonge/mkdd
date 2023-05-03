#ifndef MSL_BSEARCH_H
#define MSL_BSEARCH_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void *bsearch(const void *key, const void *base0, size_t nmemb, size_t size,
                int (*compar)(const void *, const void *));

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif