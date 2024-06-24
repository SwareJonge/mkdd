#ifndef PRINTPAL_H
#define PRINTPAL_H

#include <JSystem/JKernel/JKRHeap.h>

class PrintPal {
public:
    enum MessageID // Based of how it's structured in the file itself, might be wrong
    {
        mcShow_Dialog,
        mcSet_Option1,
        mcSet_Option2,
        mcShow_Logo,
        mcShow_Set
    };

    PrintPal(JKRHeap *heap);
    void init(MessageID);
    void draw();
    void calc(int);
    bool isMessage();
    bool isChoice();
    int getCounter();

    // Maybe swap these names
    int getState() { return _74; }
    int getOption() { return _78; }
    void setOption(int val) { _78 = val; }

private:
    // TODO
    u8 _0[0x64];
    MessageID mMsgId;
    int _68;
    int _6c;
    u8 _70[0x74 - 0x70];
    int _74;
    int _78;
    u8 _7c[0xa0 - 0x7c];
};

#endif
