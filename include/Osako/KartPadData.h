#ifndef KARTPADDATA_H
#define KARTPADDATA_H

#include "types.h"

#define KARTBTN_A 1
#define KARTBTN_B 2
#define KARTBTN_X 4
#define KARTBTN_Y 8
#define KARTBTN_L 16
#define KARTBTN_R 32
#define KARTBTN_Z 64
#define KARTBTN_START 128

// AND input and mask, if result is not 0, OR output with output/target mask
#define ConvertBtn(output, input, inputmask, outputmask) \
    output |= (input & inputmask) ? outputmask : 0;

struct KartPadData {
    int mStickX : 5;
    int mStickY : 3;
    u8 mButtons;
};

#endif