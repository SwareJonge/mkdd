#include <dolphin/string.h>
#include <JSystem/JSupport/JSUStream.h>
#include "types.h"

// Unused File, however is listed in some unreferenced duplicates

JSUOutputStream::~JSUOutputStream() { }

int JSUOutputStream::write(const void* data, long length)
{
	int len = writeData(data, length);
	if (len != length) {
		setState(State_1);
	}
	return len;
}

void JSUOutputStream::write(const char* str)
{
	if (str == nullptr) {
		u16 val = 0;
		if (writeData(&val, 2) != sizeof(val)) {
			setState(State_1);
		}
	} else {
		int len = strlen((char*)str);
		if ((s32)len >= 0x10000) {
			setState(State_2);
		} else {
			u16 val = len;
			if (writeData(&val, 2) != sizeof(val) || (writeData(str, len) != len)) {
				setState(State_1);
			}
		}
	}
	return;
}
