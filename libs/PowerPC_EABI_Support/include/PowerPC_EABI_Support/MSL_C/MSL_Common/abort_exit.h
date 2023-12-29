#ifndef ABORT_EXIT_H
#define ABORT_EXIT_H

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

void abort();
void exit(int status);
void __exit(int status);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif