#include "Inagaki/GameAudioMain.h"
#include "Osako/NameEntryHelper.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "Osako/Kart2DCommon.h"
#include "Osako/ResMgr.h"

#include "JSystem/J2D/J2DPane.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

// TODO: give members proper names/descriptions

const char aDrumConv[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-!?. ";

NameEntryHelper::NameEntryHelper(J2DPane **paPane, char *aText, JKRHeap *pHeap, JUTGamePad *pGamePad, bool inScene) {
    maText = aText;
    mGameSelect = inScene;
    mGamePad = pGamePad;
    mDrums[0] = new (pHeap, 0) NameEntryDrum(paPane[0], 1, aText[0], pHeap);
    mDrums[1] = new (pHeap, 0) NameEntryDrum(paPane[1], 0, aText[1], pHeap);
    mDrums[2] = new (pHeap, 0) NameEntryDrum(paPane[2], 4, aText[2], pHeap);
    mDrumIdx = 0;
    mAction = 0;
}

void NameEntryHelper::init(char *aText) {
    mDrums[0]->init(aText[0]);
    mDrums[1]->init(aText[1]);
    mDrums[2]->init(aText[2]);
    mDrumIdx = 0;
    mAction = 0;
}

void NameEntryHelper::startEdit() {
    mAction = 1;
    mDrums[mDrumIdx]->selectIn();
}

void NameEntryHelper::edit() {
    if (mGamePad->testButton(JUTGamePad::MAINSTICK_UP)) {
        mDrums[mDrumIdx]->scrollUp();
    } else if (mGamePad->testButton(JUTGamePad::MAINSTICK_DOWN)) {
        mDrums[mDrumIdx]->scrollDown();
    }

    if (mGamePad->testTrigger(JUTGamePad::A)) {
        mDrums[mDrumIdx]->selectOut();
        if (mDrumIdx == 2) {
            GetGameAudioMain()->startSystemSe(0x20003);

            for (int i = 0; i < 3; i++)
               maText[i] = mDrums[i]->getChar();
            
            mAction = 3;
        }
        else {
           GetGameAudioMain()->startSystemSe(0x2000c);
           mDrumIdx++;
           mDrums[mDrumIdx]->selectIn();
        }
    } else if (mGamePad->testTrigger(JUTGamePad::B)) {
        if (mDrumIdx == 0) {
            if (mGameSelect) { // in award mode, you can't exit out of the Name entry
                GetGameAudioMain()->startSystemSe(0x20005);
                mDrums[mDrumIdx]->selectOut();
                mAction = 4;
            }
        }
        else {
           GetGameAudioMain()->startSystemSe(0x2000c);
           mDrums[mDrumIdx]->selectOut();
           mDrumIdx--;
           mDrums[mDrumIdx]->selectIn();
        }
    }

}

void NameEntryHelper::calc() {
    switch(mAction) {
    case 1:
        mAction = 2;
        break;
    case 2:
        edit();
        break;
    }
    for (int i = 0; i < 3; i++) {
        mDrums[i]->calc();
    }
}

void NameEntryHelper::draw(J2DGrafContext *pGraf) {
    switch(mAction) {
        case 1:  // edit start
        case 2:  // edit
        case 3:  // finished a character
        case 4: { // finished all characters?
            for(int i = 0; i < 3; i++) {
                mDrums[i]->draw(pGraf);
            }
        }
    }
}

int NameEntryHelper::NameEntryDrum::convPattern(char c) {
    for (int i = 0; i < sizeof (aDrumConv) - 1; i++) {
        if (c == aDrumConv[i]) {
            return i;
        }
    }
    return 0;
}

char NameEntryHelper::NameEntryDrum::convChar(int no) {
#line 202
    JUT_ASSERT(no < sizeof( aDrumConv ))
    JUT_ASSERT(no >= 0)
    return aDrumConv[no];
}

NameEntryHelper::NameEntryDrum::NameEntryDrum(J2DPane *rootPane, u8 hideFlag, char c, JKRHeap *heap) {
    mPane = rootPane;
    mEntry = new (heap, 0) J2DScreen();
    mEntryReel = new (heap, 0) J2DScreen();
    _24 = 0;
    mScrollDir = 0;

    mPattern[0] = convPattern(c);
    mPattern[1] = getUpPattern(mPattern[0]);
    _60 = 0.6f;
    _64 = 0;
    _65 = 0;
    _66 = 0;

    JKRArchive *race2dArc = ResMgr::getArchive(ResMgr::mcArcRace2D);

    mEntry->setPriority("NameEntry.blo", 0x20000, race2dArc);
    mEntryReel->setPriority("NameEntryReel.blo", 0x20000, race2dArc);
    rootPane->appendChild(mEntry);
    void *bck = JKRGetNameResource("NameEntry.bck", race2dArc);
    void *btk = JKRGetNameResource("NameEntry.btk", race2dArc);
    void *brk = JKRGetNameResource("NameEntry.brk", race2dArc);
    void *reelBck = JKRGetNameResource("NameEntryReel.bck", race2dArc);
    
    mTrans = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mEntry->setAnimation(mTrans);
    _4c = 0;
    mTrans->setFrame(0.0f);
    
    mTrans2 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    J2DPicture *pPane = (J2DPicture*)mEntry->search(0x4e43547232); // 'NCTr2'
    pPane->setAnimation(mTrans2);

    _4e = 10;
    mTexKey = (J2DAnmTextureSRTKey *)J2DAnmLoaderDataBase::load(btk);
    mEntry->setAnimation(mTexKey);

    _50 = 0;
    mReelTrans = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(reelBck);
    mEntryReel->setAnimation(mReelTrans);

    _5c = 0.0f;
    mReelTrans->setFrame(0.0f);
    for (int i = 0; i < 4; i++) {
        mTevKeys[i] = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
        mTevKeys[i]->searchUpdateMaterialID(mEntry);
        _52[i] = 0;
        
        J2DPicture *pPane = (J2DPicture*)mEntry->search(0x547231ull+i); // 'Tr1'
        pPane->setAnimation(mTevKeys[i]);
        mTevKeys[i]->setFrame(0.0f);
        if (hideFlag & 1 << i) {
            pPane->hide();
        }
    }

    for (int i = 0; i < 2; i++) {
        J2DPicture *pPane = (J2DPicture*) mEntryReel->search(0x5265656c31ull+i); // 'Reel1'
#line 281
        JUT_ASSERT(pPane->getTypeID() == J2DPaneType_Picture)
        _14[i] = pPane;
        _14[i]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[i])), 0);
    }

    mNamePict = (J2DPicture *)mEntry->search(0x4e616d6531ull); // 'Name1'
    mPictSci = (J2DPicture *)mEntryReel->search(0x53636931ull); // 'Sci1'
    mEntryReel->setScissor(true);

    pPane = (J2DPicture*)mEntry->search(0x536c6f74ull); // 'Slot'
#line 292
    JUT_ASSERT(pPane->getTypeID() == J2DPaneType_Picture)
    ResTIMG *slotImg = (ResTIMG *)JKRGetNameResource("slot2.bti", race2dArc);
    pPane->changeTexture(slotImg, 0);

    // sane
    int i;
    void *lightImg;
    for (lightImg = JKRGetNameResource("light2.bti", race2dArc), i = 0; i < 2; i++) {
        J2DPicture *pPane = (J2DPicture*)mEntry->search(0x4c6967687431ull+i); // 'Light1'
#line 302
        JUT_ASSERT(pPane->getTypeID() == J2DPaneType_Picture)
        pPane->changeTexture((ResTIMG *)lightImg, 0);
    }

    pPane = (J2DPicture*)mEntry->search(0x536c6f744231ull); // 'SlotB1'
#line 309
    JUT_ASSERT(pPane->getTypeID() == J2DPaneType_Picture)
    ResTIMG *whiteImg = (ResTIMG *)JKRGetNameResource("white.bti", race2dArc);
    pPane->changeTexture(whiteImg, 0);
}

void NameEntryHelper::NameEntryDrum::init(char c) {
    _24 = 0;
    mScrollDir = 0;
    mPattern[0] = convPattern(c);
    mPattern[1] = getUpPattern(mPattern[0]);
    _60 = 0.6f;
    _64 = 0;
    _65 = 0;
    _66 = 0;

    _14[0]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[0])), 0);
    _14[1]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[1])), 0);

    _4c = 0;
    mTrans->setFrame(0.0f);
    _4e = 10;
    _50 = 0;
    _5c = 0.0f;
    mReelTrans->setFrame(0.0f);

    for (int i = 0; i < 4; i++) {
        _52[i] = 0;
        mTevKeys[i]->setFrame(0.0f);
    }
}

void NameEntryHelper::NameEntryDrum::selectIn() {
    _24 = 1;
    _4c = 0;
    _4e = 10;
}

void NameEntryHelper::NameEntryDrum::selectOut(NameEntryDrum::Arrow arrow) { } // UNUSED

void NameEntryHelper::NameEntryDrum::selectOut() {
    _24 = 3;
    _4c = 10;
}

void NameEntryHelper::NameEntryDrum::scrollUp() {
    _64 |= 2;
    _64 &= ~8;

    if (mScrollDir != 1) {
        GetGameAudioMain()->startSystemSe(0x20000);
        mScrollDir = 1;
        _66 = 1;
    }

    if (_52[1] == 0) {
        _52[1] = 1;
    }
    _65 |= 2;
    _65 &= ~8;
}

void NameEntryHelper::NameEntryDrum::scrollDown() {
    _64 |= 8;
    _64 &= ~2;

    if (mScrollDir != 2) {
        GetGameAudioMain()->startSystemSe(0x20000);
        mScrollDir = 2;
    }

    if (_52[3] == 0) {
        _52[3] = 1;
    }
    _65 |= 8;
    _65 &= ~2;
}

int NameEntryHelper::NameEntryDrum::getUpPattern(int pattern) {
    if (pattern == 0)
        return 0x29; // ')'
    return pattern - 1;
}

int NameEntryHelper::NameEntryDrum::getDownPattern(int pattern) {
    if (pattern == 0x29) {
        return 0;
    }
    return pattern + 1;
}

char NameEntryHelper::NameEntryDrum::getChar() {
    switch (mScrollDir) {
        case 0: // ?
        case 1: // Down
            return convChar(mPattern[0]);
        case 2: // Up
            return convChar(mPattern[1]);    
    }
    return '\0';
}

void NameEntryHelper::NameEntryDrum::calc() {
    mTrans->setFrame(_4c);
    mTrans2->setFrame(_4e);

    switch (_24) {
    case 1: {
        if (++_4c >= 11) {
            _24 = 2;
            _4c = 10;
        }
        break;
    }
    case 3: {
        if (--_4c < 0) {
            _24 = 0;
            _4c = 0;
        }
        break;
    }
    case 2: {
        if (++_4e >= 50) {
            _4e = 10;
        }
        break;
    }
    }

    switch (mScrollDir) {
    case 1: {
        _5c -= _60;
        if (_5c <= 0.0f) {
            if (_64 & 2) {
                _5c += 10.0f;
                if (_66 == 0) {
                    _52[1] = 1;
                    GetGameAudioMain()->startSystemSe(0x20000);
                    if (_60 < 2.5f) {
                        _60 += 0.1f;
                    }
                }

                mPattern[1] = mPattern[0];
                mPattern[0] = getDownPattern(mPattern[0]);
                _14[0]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[0])), 0);
                _14[1]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[1])), 0);
            }
            else {
                _5c = 0.0f;
                _60 = 0.6f;
                mScrollDir = 0;
            }
            _66 = 0;
        }
        _64 &= ~2;
        break;
    }
    case 2: {
        _5c += _60;
        if(_5c > 10.0f) {
            if (_64 & 8) {        
                _52[3] = 1;
                GetGameAudioMain()->startSystemSe(0x20000);
                _5c -= 10.0f;
                if (_60 < 2.5f) {
                    _60 += 0.1f;
                }
            }
            else {
                _5c = 0.0f;
                _60 = 0.6f;
                mScrollDir = 0;
            }
            mPattern[0] = mPattern[1];
            mPattern[1] = getUpPattern(mPattern[1]);
            _14[0]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[0])), 0);
            _14[1]->changeTexture(Kart2DCommon::ptr()->getAsciiTexture(convChar(mPattern[1])), 0);
        }
        _64 &= ~8;
        break;
    }
    }

    mReelTrans->setFrame(_5c);

    for (int i = 0; i < 4; i++) {
        if (_52[i] > 0) {
            if (++_52[i] >= 20) {
                if (_65 & 1 << i) {
                    _52[i] = 1;
                } else {
                    _52[i] = 0;
                }
            }
            mTevKeys[i]->setFrame(_52[i]);
        }

    }

    _65 = 0;
    mTexKey->setFrame(_50);

    if (++_50 >= 120) {
        _50 = 0;
    }

    mEntry->animation();
    mEntryReel->animation();
}

void NameEntryHelper::NameEntryDrum::draw(J2DGrafContext *pGraf) {
    const JGeometry::TBox2f& bounds = mNamePict->getGlobalBounds();
    mEntryReel->draw(bounds.i.x, bounds.i.y, pGraf);
}

#include "JSystem/JAudio/JASFakeMatch2.h"
