#include "types.h"
#include "mem.h"

void *memmove(void *dst, const void *src, size_t len)
{
	const char *csrc;
	char *cdst;

	int reverse = (u32)src < (u32)dst;

	if (len >= 32)
	{
		if (((int)dst ^ (int)src) & 3)
		{
			if (!reverse)
			{
				__copy_longs_unaligned(dst, src, len);
			}
			else
			{
				__copy_longs_rev_unaligned(dst, src, len);
			}
		}
		else
		{
			if (!reverse)
			{
				__copy_longs_aligned(dst, src, len);
			}
			else
			{
				__copy_longs_rev_aligned(dst, src, len);
			}
		}

		return dst;
	}
	else
	{
		if (!reverse)
		{
			for (csrc = (const char *)src - 1, cdst = (char *)dst - 1, len++; --len;)
			{
				*++cdst = *++csrc;
			}
		}
		else
		{
			for (csrc = (const char *)src + len, cdst = (char *)dst + len, len++; --len;)
			{
				*--cdst = *--csrc;
			}
		}
	}

	return dst;
}

void *memchr(const void *src, int val, size_t n)
{
	const u8 *p;
	u32 v = val & 0xFF;

	for (p = (u8 *)src - 1, n++; --n;)
	{
		if ((*++p & 0xFF) == v)
		{
			return (void *)p;
		}
	}

	return NULL;
}

void *__memrchr(const void *src, int val, size_t n)
{
	const u8 *p;
	u32 v = val & 0xFF;

	for (p = (u8 *)src + n, n++; --n;)
	{
		if (*--p == v)
		{
			return (void *)p;
		}
	}

	return NULL;
}

int memcmp(const void *src1, const void *src2, size_t n)
{
	const u8 *p1;
	const u8 *p2;

	for (p1 = (const u8 *)src1 - 1, p2 = (const u8 *)src2 - 1, n++; --n;)
	{
		if (*++p1 != *++p2)
		{
			return (*p1 < *p2) ? -1 : 1;
		}
	}

	return 0;
}
