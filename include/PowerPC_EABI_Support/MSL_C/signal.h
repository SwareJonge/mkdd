#ifndef SIGNAL_H
#define SIGNAL_H

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

typedef void (*__signal_func_ptr)(int);

int raise(int);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif // SIGNAL_H