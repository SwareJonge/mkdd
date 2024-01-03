#ifndef CARDAGENT_H
#define CARDAGENT_H

#include "Kameda/PrintMemoryCard.h"
#include "Osako/clock.h"
#include "Osako/GhostFile.h"
#include "Osako/SaveFile.h"
#include "Osako/SystemFile.h"

namespace CardAgent
{ // Autogenerated
    // Global
    enum Command
    {
        mcCommand0,
        mcCommand1,
        mcCommand2,
        mcCommand3,
        mcCommand4,
        mcCommand5,
        mcCommand6,
        mcCommand7,
    };

    struct MessageTimer
    {
        MessageTimer()
        {
            mTime = 0;
            _4 = 0;
            _c = 0;
        }
        // fabricated, probably is set up in 2 parts
        void set()
        {
            _c = 0;
            _4 = 0;
            if (_4 == 1)
            {
                return;
            }
            mTime = Clock::getTime();
            _c = 1;
        }

        int mTime;
        u32 _4;
        f32 _8;
        u8 _c;
    };

    void create(JKRHeap *heap);      // 0x80202a80
    void ask(Command cmd, s32 chan); // 0x80202ad4
    void draw();                     // 0x80202cc0
    void calc();                     // 0x80202d08
    void waitMessage();              // 0x80202de0
    void waitButton();               // 0x80202f00
    void waitSelect();               // 0x80203010
    void check0();                   // 0x802033c8
    void waitMount();                // 0x80203508
    void waitCheck();                // 0x80203674
    void waitFormat();               // 0x8020380c
    void checkFree();                // 0x80203944
    void open();                     // 0x80203a6c
    void waitCreate();               // 0x80203d48
    void waitWrite();                // 0x80203f6c
    void waitRead();                 // 0x802041a8
    void waitRename();               // 0x80204564
    void waitDelete();               // 0x80204708
    void waitSetStatus();            // 0x8020485c
    void waitQuit();                 // 0x80204b70
    void reset();                    // 0x80204c00
    bool isGhostSavable();           // 0x80204c18
    void errorNoCard();              // 0x80204ca8
    void errorWrongDevice();         // 0x80204d68
    void errorIOError();             // 0x80204e28
    void errorEncoding();            // 0x80204eec

    extern MessageTimer msMessageTimer;         // 0x80400cc0
    extern Command msCommand;                   // 0x804169d0
    extern int msState;                         // 0x804169d4
    extern int msSelectAt;                      // 0x804169d8
    extern int msButtonAt;                      // 0x804169dc
    extern int msMessageAt;                     // 0x804169e0
    extern int msResult;                        // 0x804169e4
    extern s32 msChan;                          // 0x804169e8
    extern SaveFile *mspSaveFile;               // 0x804169ec
    extern SystemFile *mspSystemFile;           // 0x804169f0
    extern GhostFile *mspGhostFile;             // 0x804169f4
    extern u8 msFlags;                          // 0x804169f8
    extern PrintMemoryCard *mspPrintMemoryCard; // 0x804169fc
    // Inline/Unused
    extern int msDrawFlag;

    inline int getState() { return msState; }
    inline PrintMemoryCard *getPrintMemoryCard() { return mspPrintMemoryCard; }
}; // namespace CardAgent

#endif // CARDAGENT_H
