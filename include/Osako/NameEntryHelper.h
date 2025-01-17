#ifndef NAMEENTRYHELPER_H
#define NAMEENTRYHELPER_H

#include <JSystem/J2D/J2DPicture.h>
#include <JSystem/JUtility/JUTGamePad.h>
#include "JSystem/J2D/J2DAnmLoader.h"

class NameEntryHelper { // Autogenerated
public:
    NameEntryHelper(J2DPane **, char *, JKRHeap *, JUTGamePad *, bool); // 0x80211514
    void init(char *); // 0x80211600
    void startEdit(); // 0x8021165c
    void edit(); // 0x80211690
    void calc(); // 0x8021181c
    void draw(J2DGrafContext *); // 0x80211890
    
    class NameEntryDrum {
    public:
        enum Arrow {

        };

        NameEntryDrum(J2DPane *, u8, char input, JKRHeap *); // 0x802118f8
        void init(char input); // 0x80211f98
        void selectIn(); // 0x802121d8
        void selectOut(); // 0x802121f4
        void scrollUp(); // 0x80212208
        void scrollDown(); // 0x80212298
        int getUpPattern(int); // 0x80212324
        char getChar(); // 0x80212338
        void calc(); // 0x8021248c
        void draw(J2DGrafContext *); // 0x80212ac8
        // Inline/Unused
        int convPattern(char);
        char convChar(int no);
        void selectOut(Arrow);
        int getDownPattern(int);
    public:
        J2DPane *mPane;               // 00
        J2DPicture *mNamePict;        // 04
        J2DPicture *mPictSci;         // 08
        J2DScreen *mEntry;            // 0C
        J2DScreen *mEntryReel;        // 10
        J2DPicture *_14[2];           // 14
        int mPattern[2];              // 1C
        int _24;                      // 24
        int mScrollDir;               // 28
        J2DAnmTransform *mTrans;      // 2C
        J2DAnmTransform *mTrans2;     // 30
        J2DAnmTextureSRTKey *mTexKey; // 34
        J2DAnmTevRegKey *mTevKeys[4]; // 38
        J2DAnmTransform *mReelTrans;  // 48
        s16 _4c;
        s16 _4e;
        s16 _50;
        s16 _52[4];
        f32 _5c;
        f32 _60;
        u8 _64;
        u8 _65;
        u8 _66;

        
    }; // class NameEntryDrum

    NameEntryDrum *mDrums[3];
    char *maText;
    s16 mDrumIdx;
    bool mGameSelect;
    int mAction;
    JUTGamePad *mGamePad;
}; // class NameEntryHelper
#endif // NAMEENTRYHELPER_H
