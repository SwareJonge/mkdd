#ifndef _MSL_LOCALE_API_H
#define _MSL_LOCALE_API_H

#include "types.h"

typedef int (*__decode_mbyte)(wchar_t*, const char*, size_t);
typedef int (*__encode_mbyte)(char*, wchar_t);

#endif