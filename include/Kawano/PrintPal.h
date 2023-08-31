#ifndef PRINTPAL_H
#define PRINTPAL_H

#include <JSystem/JKernel/JKRHeap.h>

class PrintPal {
public:
    enum MessageID {

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
