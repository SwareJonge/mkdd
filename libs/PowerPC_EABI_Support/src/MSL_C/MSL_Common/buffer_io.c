#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"
#include "types.h"

void __convert_from_newlines(char *buffer, size_t *length)
{
	return;
}

void __convert_to_newlines()
{
	// UNUSED
}

void __prep_buffer(FILE *file)
{
	file->mBufferPtr = file->mBuffer;
	file->mBufferLength = file->mBufferSize;
	file->mBufferLength = file->mBufferLength - (file->mPosition & file->mBufferAlignment);
	file->mBufferPosition = file->mPosition;
	return;
}

int __flush_buffer(FILE *file, size_t *length)
{
	size_t bufferLen;
	int writeCode;
	char binmode;

	bufferLen = file->mBufferPtr - file->mBuffer;
	if (bufferLen) {
		file->mBufferLength = bufferLen;

		if (file->mMode.binary_io == 0)	{
			__convert_from_newlines(file->mBuffer, &file->mBufferLength);
		}

		writeCode = file->writeFunc(file->mHandle, file->mBuffer, &file->mBufferLength, file->ref_con);
		if (length) {
			*length = file->mBufferLength;
		}
		if (writeCode) {
			return writeCode;
		}
		file->mPosition += file->mBufferLength;
	}

	__prep_buffer(file);
	return 0;
}
