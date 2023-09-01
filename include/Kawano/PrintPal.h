#ifndef PRINTPAL_H
#define PRINTPAL_H

#include <JSystem/JKernel/JKRHeap.h>

class PrintPal {
public:
        enum MessageID // Based of how it's structured in the file itself, might be wrong
    {
        mcShow_PAL,
        mcSet_60Hz,
        mcSet_50Hz
    };

    PrintPal(JKRHeap *heap);
    void init(MessageID);
    void draw();
    void calc(int);
    bool isMessage();
    bool isChoice();
    int getCounter();

private:
    // TODO
};

#endif
