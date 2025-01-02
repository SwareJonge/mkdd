#ifndef JUGEMMAIN_H
#define JUGEMMAIN_H

#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"
#include "Shiraiwa/JugemItem.h"
#include "Shiraiwa/JugemHeadItem.h"
#include "Shiraiwa/JugemRodItem.h"
#include "Shiraiwa/AnmPlayer.h"

class TJugem : public TMapObjHioNode, public StateObserver
{
public:
    TJugem();

    void setJugemItem(TJugemItem *, u32);
    void setJugemHeadItem(TJugemHeadItem *item, u32 id);
    void setCameraNum(u8 cam);

    virtual ~TJugem() {}
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void createShadowModel(JKRSolidHeap *, u32);
    virtual void reset();
    virtual void calc();
    virtual void update();
    virtual void viewCalc(u32);
    virtual void setCurrentViewNo(u32);
    virtual const char *getBmdFileName();
    virtual const char *getJ3DModelDataTevStageNum();
    virtual void createColModel(J3DModelData *);

    virtual void MoveExec();
    virtual void InitExec();

    bool isDemoMode();

    void signalGo()
    {
        mIsAbleStart = true;
        setState(0);
    };
    bool isAbleStart(); /* { return mIsAbleStart; }*/
    bool isCallThree() { return !mSignal ? false : mSignal->isCallThree(); }
    bool isDemoOut() { return isDemoMode() && mAnmPlayer.getCurAnmNumber() == 2; }

    const JGeometry::TVec3f &getPosition() const { return mPos; }
    void setHearPtr(TJugemHeadHear *hear) { setJugemHeadItem(hear, 0); }
    void setSignalPtr(TJugemRodSignal *signal) { mSignal = signal; }
    void setVoidRodPtr(TJugemVoidRod *rod) {
        setJugemItem(rod, 1);
        setSignalPtr(rod->getSignal());
    }

    void setDemoPattern(u32 pattern) {
        mDemoPattern = pattern;
    }

    void startDemo() { setState(0x12); }
    void endDemo() { setState(0x14); }

    int getSignalState();

    void setEndingState() { setState(0x16); }

    u8 _158[0x178 - 0x158];   //
    TAnmPlayer mAnmPlayer;    // 178
    u8 _18c[0x244 - 0x18c];   //
    u32 mDemoPattern;         // 244
    u8 _248[0x250 - 0x248];   //
    bool mIsAbleStart;        // 250    
    u8 _251[0x26c - 0x251];   //
    TJugemRodSignal *mSignal; // 26c
    u8 _270[0x27c - 0x270];   //
}; // Size: 0x27c

#endif
