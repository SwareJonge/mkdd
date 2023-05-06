#include <string.h>
#include <dolphin/types.h>

void *memcpy(void *dst, const void *src, size_t n)
{
    u8 *__src;
    u8 *__dst;
    int i;

    if (src >= dst)
    {
        __src = ((u8 *)src) - 1;
        __dst = ((u8 *)dst) - 1;
        i = n + 1;
        while (--i)
        {
            *((u8 *)++__dst) = *((u8 *)++__src);
        }
        return dst;
    }
    else
    {
        __src = ((u8 *)src) + n;
        __dst = ((u8 *)dst) + n;
        i = n + 1;
        while (--i)
        {
            *((u8 *)--__dst) = *((u8 *)--__src);
        }
        return dst;
    }
}
void __fill_mem(void *dst, int c, size_t n)
{
    u8 *cdest;
    u32 *idest;
    u32 i;
    u32 cval;

    cval = (u8)c;
    cdest = (u8 *)dst - 1;

    if (n >= 32)
    {
        i = ~(u32)cdest & 3;
        if (i != 0)
        {
            n = n - i;
            do
            {
                *(++cdest) = (u8)cval;
            } while (--i);
        }
        if (cval != 0)
        {
            cval = cval << 24 | cval << 16 | cval << 8 | cval;
        }

        idest = (u32 *)(cdest - 3);
        i = n >> 5;
        if (i != 0)
        {
            do
            {
                idest[1] = cval;      // 4
                idest[2] = cval;      // 8
                idest[3] = cval;      // c
                idest[4] = cval;      // 10
                idest[5] = cval;      // 14
                idest[6] = cval;      // 18
                idest[7] = cval;      // 1c
                *(idest += 8) = cval; // 20
            } while (--i);
        }

        i = (n >> 2) & 7;

        if (i != 0)
        {
            do
            {
                *++idest = cval;
            } while (--i);
        }

        cdest = (u8 *)idest + 3;
        n &= 3;
    }
    if (n != 0)
    {
        do
        {
            *++cdest = (u8)cval;
        } while (--n);
    }
}

void *memset(void *dst, int c, size_t n)
{
    __fill_mem(dst, c, n);
    return dst;
}