#include "JSystem/JAudio/System/JASSeqCtrl.h"
#include "JSystem/JAudio/System/JASSeqParser.h"
#include "JSystem/JUtility/JUTAssert.h"

JASSeqParser JASSeqCtrl::sDefaultParser;

JASSeqCtrl::JASSeqCtrl() {
    mParser = &sDefaultParser;
    mWaitTime = 0;
    _44 = NULL;
    mIntrTable = 0;
    mIntrBits = 0;
    mIntrMask = 0;
    _50 = 0;
    _51 = false;
    _54 = 0;
    _58 = 0;
    mReader.init(); // already called in implicit ctor
}

void JASSeqCtrl::init() {
    mReader.init();
    mParser = &sDefaultParser;
    mWaitTime = 0;
    _44 = NULL;
    mIntrTable = 0;
    mIntrBits = 0;
    mIntrMask = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    _51 = false;
}

void JASSeqCtrl::start(void *base, u32 offset) {
    mReader.init(base);
    mReader.jump(offset);
}

int JASSeqCtrl::tickProc(JASTrack *track) {
    if (!mReader.getBase()) {
        return 0;
    }

    interrupt(INTRTYPE_VALUE_6);
    timerProcess();

    if (_51) {
        if (!track->checkNoteStop(0)) {
            return 0;
        }

        _51 = false;
    }

    if (mWaitTime > 0) {
        mWaitTime--;
    }

    checkIntr();

    while ((mWaitTime == 0 || _44 != NULL) && !_51) {
        if (mParser->parse(track) < 0) {
            return -1;
        }
    }
    
    return 0;
}

void JASSeqCtrl::interrupt(JASSeqCtrl::IntrType intrType) {
    if (mIntrMask & (1 << intrType)) {
        mIntrBits |= (1 << intrType);
    }
}

void JASSeqCtrl::setIntrMask(u32 mask) {
    mIntrMask |= mask;
}

void JASSeqCtrl::clrIntrMask(u32 mask) {
    mIntrMask &= ~mask;
}

bool JASSeqCtrl::retIntr() {
    if (_44 == NULL)
        return false;
    
    mReader.jump(_44);
    _44 = NULL;
    return true;
}

int JASSeqCtrl::findIntr() {
    u32 intr = mIntrMask & mIntrBits;
    for (int i = 0; intr != 0; intr >>= 1, i++) {
        if (intr & 1) {
             mIntrBits &= ~(1 << i);
            return i;
        }
    }
    return -1;
}

void JASSeqCtrl::checkIntr() {
    if (_44 != NULL)
        return;

    int intr = findIntr();
    if (intr < 0)
        return;

#line 143
    u32 vec = mIntrTable;
    JUT_ASSERT(vec != 0);
    vec += intr * 3;
    u32 pos = mReader.get24(vec);
    _44 = mReader.getCur();
    mReader.jump(pos);
}

void JASSeqCtrl::timerProcess() {
    if (_54 == 0) 
        return;
    
    _54--;
    if (_54) 
        return;
    
    interrupt(INTRTYPE_VALUE_5);

    if (_50) {
        _50--;
        if (_50) {
            _54 = _58;
        }
    } else {
        _54 = _58;
    }
}

#include "JSystem/JAudio/JASFakeMatch.h"
