#include "types.h"

#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"

void putwc(void)
{
	// UNUSED FUNCTION
}

void putwchar(void)
{
	// UNUSED FUNCTION
}

void fputwc(void)
{
	// UNUSED FUNCTION
}

void getwc(void)
{
	// UNUSED FUNCTION
}

void getwchar(void)
{
	// UNUSED FUNCTION
}

void fgetwc(void)
{
	// UNUSED FUNCTION
}

void ungetwc(void)
{
	// UNUSED FUNCTION
}

void fputws(void)
{
	// UNUSED FUNCTION
}

void fgetws(void)
{
	// UNUSED FUNCTION
}

int fwide(FILE* stream, int mode)
{
	int res;
	int orientation;

	if (stream == nullptr || stream->mMode.file_kind == __closed_file)
		return 0;

	orientation = stream->mMode.file_orientation;
	switch (orientation) {
	case __unoriented:
		if (mode > 0)
			stream->mMode.file_orientation = __wide_oriented;
		else if (mode < 0)
			stream->mMode.file_orientation = __char_oriented;

		res = mode;
		break;

	case __wide_oriented:
		res = 1;
		break;

	case __char_oriented:
		res = -1;
		break;
	}
	return res;
}
