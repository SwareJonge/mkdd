#ifndef DOLPHIN_LG_H
#define DOLPHIN_LG_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C"
{
#endif // ifdef __cplusplus

struct LGPosition { // seems really similar to PADStatus but is slightly different
    u16 input;
    s8 _2;
    s8 stick3;
    u8 analogA;
    u8 analogB;
    u8 _6; // unknown, are there any other analog buttons?(start maybe)
    u8 analogL;
    u8 analogR;
    s8 err; // -1 probably means disconnected?
};

#ifdef __cplusplus
}
#endif

#endif