#include "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h"

int abs(int x)
{
	if (x < 0)
		return -x;
	else
		return x;
}

long labs(long x)
{
	// UNUSED FUNCTION
	if (x < 0)
		return -x;
	else
		return x;
}

void llabs(void)
{
	// UNUSED FUNCTION
}

div_t div(s32 __numer, s32 __denom)
{
	// UNUSED FUNCTION
	int iVar1;
	int iVar2;
	int iVar3;
	div_t ret;

	iVar2 = 1;
	iVar3 = 1;
	if (__numer < 0) {
		__numer = -__numer;
		iVar2   = -1;
	}
	if (__denom < 0) {
		__denom = -__denom;
		iVar3   = -1;
	}
	iVar1 = (__numer / __denom) * (iVar2 * iVar3);

	ret.quot = iVar1;
	ret.rem  = __numer * iVar2 - iVar3 * (iVar1 * __denom);
	return ret;
}


void ldiv(void)
{
	// UNUSED FUNCTION
}

void lldiv(void)
{
	// UNUSED FUNCTION
}

void __msl_add(void)
{
	// UNUSED FUNCTION
}

void __msl_ladd(void)
{
	// UNUSED FUNCTION
}

void __lladd(void)
{
	// UNUSED FUNCTION
}

void __msl_mul(void)
{
	// UNUSED FUNCTION
}

void __msl_lmul(void)
{
	// UNUSED FUNCTION
}

void __llmul(void)
{
	// UNUSED FUNCTION
}

void __msl_div(void)
{
	// UNUSED FUNCTION
}

void __msl_ldiv(void)
{
	// UNUSED FUNCTION
}

void __lldiv(void)
{
	// UNUSED FUNCTION
}

void __msl_mod(void)
{
	// UNUSED FUNCTION
}

void __msl_lmod(void)
{
	// UNUSED FUNCTION
}

void __llmod(void)
{
	// UNUSED FUNCTION
}
