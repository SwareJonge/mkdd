#include "types.h"

s32 InitializeUART(u32);                      /* extern */
s32 OSGetConsoleType();                       /* extern */
s32 WriteUARTN(s32, s32);                     /* extern */
s32 __TRK_write_console(s32, s32, s32*, s32); /* extern */

BOOL __write_console(s32 arg0, s32 arg1, s32* arg2, s32 arg3)
{

	if ((OSGetConsoleType() & 0x20000000) == 0) {
		if (__init_uart_console() != 0) {
			return TRUE;
		}
		if (WriteUARTN(arg1, *arg2) != 0) {
			*arg2 = 0;
			return TRUE;
		}
	}
	__TRK_write_console(arg0, arg1, arg2, arg3);
	return FALSE;
}

int __close_console()
{
	return 0;
}

int __init_uart_console(void)
{
	static BOOL initialized;
	int ret = 0;

	if (initialized == 0) {
		ret = InitializeUART(0xE100);
		if (ret == 0) {
			initialized = 1;
		}
	}

	return ret;
}

void __delete_file(void)
{
	// UNUSED FUNCTION
}

void __rename_file(void)
{
	// UNUSED FUNCTION
}

void __temp_file_name(void)
{
	// UNUSED FUNCTION
}
