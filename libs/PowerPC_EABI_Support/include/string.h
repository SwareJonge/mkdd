#ifndef _STRING_H
#define _STRING_H

#include "types.h"
#include "mem.h"
#include "extras.h"
#ifdef __cplusplus
extern "C" {
#endif

char* strcpy(char*, const char*);
char* strncpy(char*, const char*, size_t);

char* strcat(char*, const char*);
char* strncat(char*, const char*, size_t);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);
char* strchr(const char*, int);
char* strstr(const char*, const char*);
char* strrchr(const char* str, int chr);
size_t strlen(const char*);
long strtol(const char *str, char **end, int base);

int __msl_strnicmp(const char *s1, const char *s2, int n);

#ifdef __cplusplus
}
#endif
#endif
