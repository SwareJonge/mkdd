#ifndef _MSL_STRTOUL_H
#define _MSL_STRTOUL_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned long strtoul(const char* str, char** end, int base);
unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned, int* negative,
                        int* overflow);
unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned,
                              int* negative, int* overflow);
int atoi(const char* str);

#ifdef __cplusplus
}
#endif

#endif
