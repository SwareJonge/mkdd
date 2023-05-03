#include "types.h"

#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/critical_regions.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h"
#include "errno.h"

// define standard C file pointer location names
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

int _ftell(FILE* file)
{
	int charsInUndoBuffer = 0;
	int position;

	u8 tmp_kind = file->mMode.file_kind;
	if (!(tmp_kind == __disk_file || tmp_kind == __console_file) || file->mState.error) {
		errno = 0x28;
		return -1;
	}

	if (file->mState.io_state == __neutral)
		return (file->mPosition);

	position = file->mBufferPosition + (file->mBufferPtr - file->mBuffer);

	if (file->mState.io_state >= __rereading) {
		charsInUndoBuffer = file->mState.io_state - __rereading + 1;
		position -= charsInUndoBuffer;
	}

	// got added in later it seems?
	/*if (!file->mMode.binary_io) {
		int n = file->mBufferPtr - file->mBuffer - charsInUndoBuffer;
		u8* p = (u8*)file->mBuffer;

		while (n--)
			if (*p++ == '\n')
				position++;
	}*/

	return (position);
}

int ftell(FILE* stream)
{
	int retval;

	__begin_critical_region(stdin_access);
	retval = (long)_ftell(stream);
	__end_critical_region(stdin_access);
	return retval;
}

int _fseek(FILE *file, fpos_t offset, int whence)
{
	fpos_t pos;
	__pos_proc func;

	unsigned char fileKind = file->mMode.file_kind;
	if (fileKind != 1 || file->mState.error != 0) {
		errno = 0x28;
		return -1;
	}

	if (file->mState.io_state == 1) {
		if (__flush_buffer(file, nullptr) != 0) {
			file->mState.error  = 1;
			file->mBufferLength = 0;
			errno                = 0x28;
			return -1;
		}
	}

	if (whence == SEEK_CUR) {
		whence = SEEK_SET;

		if ((pos = _ftell(file)) < 0)
			pos = 0;

		offset +=  pos;
	}

	if ((whence != SEEK_END) && (file->mMode.io_mode != 3) && (file->mState.io_state == 2 || file->mState.io_state == 3)) {
		if ((offset >= file->mPosition) || !(offset >= file->mBufferPosition)) {
			file->mState.io_state = 0;
		} else {
			file->mBufferPtr      = file->mBuffer + (offset - file->mBufferPosition);
			file->mBufferLength   = file->mPosition - offset;
			file->mState.io_state = 2;
		}
	} else {
		file->mState.io_state = 0;
	}

	if (file->mState.io_state == 0) {

		if ((func = file->positionFunc) != nullptr && func(file->mHandle, &offset, whence, file->ref_con) != 0)
		{
			file->mState.error = 1;
			file->mBufferLength = 0;
			errno = 0x28;
			return -1;
		}
		else
		{
			file->mState.eof    = 0;
			file->mPosition     = offset;
			file->mBufferLength = 0;
		}
	}

	return 0;
}

int fseek(FILE *stream, fpos_t offset, int whence)
{
	fpos_t start;
	int code;
	start = offset;
	__begin_critical_region(stdin_access);
	code = _fseek(stream, start, whence); // 0 if successful, -1 if error
	__end_critical_region(stdin_access);
	return code;
}
