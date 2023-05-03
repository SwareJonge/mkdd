extern void (*__stdio_exit)(void);

extern void __close_all(void);

void clearerr(void)
{
	// UNUSED FUNCTION
}

void feof(void)
{
	// UNUSED FUNCTION
}

void ferror(void)
{
	// UNUSED FUNCTION
}

void perror(void)
{
	// UNUSED FUNCTION
}

void __stdio_atexit(void) { __stdio_exit = __close_all; }
