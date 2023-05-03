#include "types.h"
#include "ctype.h"

void strdup(void)
{
	// UNUSED FUNCTION
}

void _strdup(void)
{
	// UNUSED FUNCTION
}

void strlwr(void)
{
	// UNUSED FUNCTION
}

void _strlwr(void)
{
	// UNUSED FUNCTION
}

void ultoa(void)
{
	// UNUSED FUNCTION
}

void _ultoa(void)
{
	// UNUSED FUNCTION
}

void gcvt(void)
{
	// UNUSED FUNCTION
}

void _gcvt(void)
{
	// UNUSED FUNCTION
}

void heapmin(void)
{
	// UNUSED FUNCTION
}

void _heapmin(void)
{
	// UNUSED FUNCTION
}

int stricmp(char* param_1, char* param_2)
{
	s8 a_var;
	s8 b_var;

	do {
		a_var = *param_1;
		param_1++;
		b_var = _tolower(a_var);

		a_var = *param_2;
		param_2++;
		a_var = _tolower(a_var);

		if (b_var < a_var) {
			return -1;
		}
		if (b_var > a_var) {
			return 1;
		}
	} while (b_var != 0);
	return 0;
}

void _stricmp(void)
{
	// UNUSED FUNCTION
}

int strnicmp(const char *s1, const char *s2, int n)
{
	return __msl_strnicmp(s1, s2, n);
}

void _strnicmp(void)
{
	// UNUSED FUNCTION
}

void strupr(void)
{
	// UNUSED FUNCTION
}

void _strupr(void)
{
	// UNUSED FUNCTION
}

void strdate(void)
{
	// UNUSED FUNCTION
}

void _strdate(void)
{
	// UNUSED FUNCTION
}

void strset(void)
{
	// UNUSED FUNCTION
}

void _strset(void)
{
	// UNUSED FUNCTION
}

void strnset(void)
{
	// UNUSED FUNCTION
}

void _strnset(void)
{
	// UNUSED FUNCTION
}

void strspnp(void)
{
	// UNUSED FUNCTION
}

void _strspnp(void)
{
	// UNUSED FUNCTION
}

void strncasecmp(void)
{
	// UNUSED FUNCTION
}

void _strncasecmp(void)
{
	// UNUSED FUNCTION
}

void strcmpi(void)
{
	// UNUSED FUNCTION
}

void _strcmpi(void)
{
	// UNUSED FUNCTION
}

void strncmpi(void)
{
	// UNUSED FUNCTION
}

void _strncmpi(void)
{
	// UNUSED FUNCTION
}

void strcasecmp(void)
{
	// UNUSED FUNCTION
}

void _strcasecmp(void)
{
	// UNUSED FUNCTION
}

void _stricoll(void)
{
	// UNUSED FUNCTION
}

void _strncoll(void)
{
	// UNUSED FUNCTION
}

void _strnicoll(void)
{
	// UNUSED FUNCTION
}

void stricoll(void)
{
	// UNUSED FUNCTION
}

void strncoll(void)
{
	// UNUSED FUNCTION
}

void strnicoll(void)
{
	// UNUSED FUNCTION
}

void itoa(void)
{
	// UNUSED FUNCTION
}

void _itoa(void)
{
	// UNUSED FUNCTION
}

void strrev(void)
{
	// UNUSED FUNCTION
}

void _strrev(void)
{
	// UNUSED FUNCTION
}

void filelength(void)
{
	// UNUSED FUNCTION
}

void _filelength(void)
{
	// UNUSED FUNCTION
}

void chsize(void)
{
	// UNUSED FUNCTION
}

void _chsize(void)
{
	// UNUSED FUNCTION
}

void wtoi(void)
{
	// UNUSED FUNCTION
}

void _wtoi(void)
{
	// UNUSED FUNCTION
}

void wcslwr(void)
{
	// UNUSED FUNCTION
}

void _wcslwr(void)
{
	// UNUSED FUNCTION
}

void wcsupr(void)
{
	// UNUSED FUNCTION
}

void _wcsupr(void)
{
	// UNUSED FUNCTION
}

void wcsicmp(void)
{
	// UNUSED FUNCTION
}

void _wcsicmp(void)
{
	// UNUSED FUNCTION
}

void wcsnicmp(void)
{
	// UNUSED FUNCTION
}

void _wcsnicmp(void)
{
	// UNUSED FUNCTION
}

void wcsrev(void)
{
	// UNUSED FUNCTION
}

void _wcsrev(void)
{
	// UNUSED FUNCTION
}

void wcsset(void)
{
	// UNUSED FUNCTION
}

void _wcsset(void)
{
	// UNUSED FUNCTION
}

void wcsnset(void)
{
	// UNUSED FUNCTION
}

void _wcsnset(void)
{
	// UNUSED FUNCTION
}

void wcsspnp(void)
{
	// UNUSED FUNCTION
}

void _wcsspnp(void)
{
	// UNUSED FUNCTION
}

void wcsdup(void)
{
	// UNUSED FUNCTION
}

void _wcsdup(void)
{
	// UNUSED FUNCTION
}

void wstrrev(void)
{
	// UNUSED FUNCTION
}

void _wstrrev(void)
{
	// UNUSED FUNCTION
}

void _wcsicoll(void)
{
	// UNUSED FUNCTION
}

void _wcsncoll(void)
{
	// UNUSED FUNCTION
}

void _wcsnicoll(void)
{
	// UNUSED FUNCTION
}

void wcsicoll(void)
{
	// UNUSED FUNCTION
}

void wcsncoll(void)
{
	// UNUSED FUNCTION
}

void wcsnicoll(void)
{
	// UNUSED FUNCTION
}

void itow(void)
{
	// UNUSED FUNCTION
}

void _itow(void)
{
	// UNUSED FUNCTION
}
