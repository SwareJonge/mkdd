#include <string.h>
#include <JSystem/JSupport/JSUStream.h>
#include "types.h"

void JSUMemoryInputStream::setBuffer(const void* buffer, long length)
{
	mObject   = buffer;
	mLength   = length;
	mPosition = 0;
}

int JSUMemoryInputStream::readData(void* data, long length)
{
	if (mPosition + length > mLength) {
		length = mLength - mPosition;
	}
	if (length > 0) {
		memcpy(data, (void*)((int)mObject + mPosition), length);
		mPosition += length;
	}
	return length;
}

int JSUMemoryInputStream::seekPos(long offset, JSUStreamSeekFrom mode)
{
	u32 originalPosition = mPosition;
	switch (mode) {
	case SEEK_SET:
		mPosition = offset;
		break;
	case SEEK_END:
		mPosition = mLength - offset;
		break;
	case SEEK_CUR:
		mPosition += offset;
		break;
	default:
		break;
	}
	if (0 > mPosition) {
		mPosition = 0;
	}
	if (mPosition > mLength) {
		mPosition = mLength;
	}
	return mPosition - originalPosition;
}
