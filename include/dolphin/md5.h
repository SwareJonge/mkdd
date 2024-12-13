#ifndef __DOLPHIN_OS_MD5_H__
#define __DOLPHIN_OS_MD5_H__

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MD5.H - header file for MD5C.C */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */

/* MD5 context. */
typedef struct MD5Context MD5_CTX;
typedef struct MD5Context MD5Context;
struct MD5Context
{
    u32 state[4];       // state (ABCD)
    u32 count[2];       // number of bits, modulo 2^64 (lsb first)
    u8  buffer[64];     // input buffer
};

void MD5Init  (MD5Context* context);
void MD5Update(MD5Context* context, u8* input, u32 inputLen);
void MD5Final (u8 digest[16], MD5Context* context);

#ifdef __cplusplus
}
#endif

#endif
