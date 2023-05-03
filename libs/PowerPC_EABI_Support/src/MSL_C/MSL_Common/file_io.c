#include "types.h"
#include "string.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"

int fclose(FILE* file)
{
	int flush_result, close_result;

	if (file == nullptr)
		return (-1);
	if (file->mMode.file_kind == __closed_file)
		return (0);

	flush_result = fflush(file);

	close_result = (*file->closeFunc)(file->mHandle);

	file->mMode.file_kind = __closed_file;
	file->mHandle         = 0;

	if (file->mState.free_buffer)
		free(file->mBuffer);
	return ((flush_result || close_result) ? -1 : 0);
}

int fflush(FILE* file)
{
	u32 pos;

	if (file == nullptr) {
		return __flush_all();
	}

	if (file->mState.error != 0 || file->mMode.file_kind == __closed_file) {
		return -1;
	}

	if (file->mMode.io_mode == 1) {
		return 0;
	}

	if (file->mState.io_state >= 3) {
		file->mState.io_state = 2;
	}

	if (file->mState.io_state == 2) {
		file->mBufferLength = 0;
	}

	if (file->mState.io_state != 1) {
		file->mState.io_state = 0;
		return 0;
	}

	if (file->mMode.file_kind != __disk_file || (pos = ftell(file)) < 0)
		pos = 0;

	if (__flush_buffer(file, 0) != 0) {
		file->mState.error  = 1;
		file->mBufferLength = 0;
		return -1;
	}

	file->mState.io_state = 0;
	file->mPosition       = pos;
	file->mBufferLength   = 0;
	return 0;
}

int __msl_strnicmp(const char *s1, const char *s2, int n)
{
	int i;
	char c1, c2;

	for (i = 0; i < n; i++) {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);

		if(c1 < c2) {
			return -1;
		}

		if (c1 > c2) {
			return 1;
		}

		if(c1 == '\0') {
			return 0;
		}
	}
	return 0;
	
}
