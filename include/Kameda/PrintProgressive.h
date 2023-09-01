#ifndef PRINTPROGRESSIVE_H
#define PRINTPROGRESSIVE_H

#include <JSystem/JKernel/JKRHeap.h>
#include "types.h"

class PrintProgressive
{
public:
    enum MessageID // according to debug test screen
    {
        mcShow_Progressive,
        mcSet_Progressive,
        mcSet_Interlaced,
        mcShow_Logo
    };
    PrintProgressive(JKRHeap *); // 0x8016aac4
    void init(MessageID);        // 0x8016b138
    void draw();                 // 0x8016b65c
    void calc(int);              // 0x8016b6dc
    void isMessage();            // 0x8016bc28
    void isChoice();             // 0x8016bcbc
    void getCounter();           // 0x8016bd48
    // Inline/Unused
    // void ~PrintProgressive();
    // void calc_bck();
    // void calc_texChange();
    // void setCounter(int);
private:
    // TODO
};

#endif // PRINTPROGRESSIVE_H
