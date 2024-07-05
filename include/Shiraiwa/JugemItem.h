#ifndef JUGEMITEM_H
#define JUGEMITEM_H

#include "Shiraiwa/JugemRodItem.h"

class TJugemItem : public GeographyObj {
public:
    TJugemItem(u32 id) : GeographyObj(id) {
        _14c = 0;
    }
    virtual ~TJugemItem() {}
    virtual void calc() {} 
    virtual void setPosition(const JGeometry::TVec3f &pos) { mPos.set(pos); } // 80
    virtual void setRMtx(const JGeometry::TPos3f &rMtx) { mRotMtx.set(rMtx);  } // 84
    virtual void hideModel(u32 p1) { mModel.clipAll(p1, false); } // 88
    virtual void show() { mGeoObjFlag &= ~0x20; } // 8c
    virtual void hide() { mGeoObjFlag |= 0x20; } // 90
    virtual void hideAll() { hide(); } // 94
    virtual void hold(u32) {} // 98
    virtual void changeAnimation(u32 typre) // 9C TODO: OK?
    {
        if(mAnmCtrl == nullptr)
            return;
        
        if(mAnmCtrl->IsAvailableTrans())
            mAnmCtrl->StopTrans();
    }

protected:
    int _14c;
};

class TJugemVoidRod : public TJugemItem {
public:
    TJugemVoidRod(); // 0x80291404
    ~TJugemVoidRod(); // 0x80291630
    virtual void reset(); // 0x8029175c
    virtual void loadAnimation(); // 0x802917e4
    virtual const char *getShadowBmdFileName(); // 0x80291890
    virtual const char *getBmdFileName(); // 0x80291898
    virtual void createModel(JKRSolidHeap *, u32); // 0x802918c0
    virtual void calc(); // 0x802919dc
    virtual void createColModel(J3DModelData *); // 0x80291a0c
    virtual void setPosition(const JGeometry::TVec3f &); // 0x80291a10
    virtual void setRMtx(const JGeometry::TPos3f &); // 0x80291a2c
    virtual void hideModel(u32); // 0x80291a60
    virtual void show(u8); // 0x80291ab4
    virtual void update(); // 0x80291b08
    virtual void hide(); // 0x80291b58
    virtual void hideAll(); // 0x80291b9c
    virtual void hold(u32); // 0x80291cb4
    virtual void changeAnimation(u32); // 0x80291d74
   
    void setJugemRodItem(TJugemRodItem *, u32); // 0x80291c34
    void nodeCallBack(J3DJoint *, int); // 0x80291ecc

    TJugemRodSignal *getSignal() const { return mSignal; }

    //void sAnmInfos_Rod_Lap; // 0x803a3ac0
    //void sAnmInfos_Rod_Start; // 0x803a3b10
    //void sAnmInfos_Rod_Reverse; // 0x803a3b74
    //void sAnmInfos_Rod_Rescue; // 0x803a3b9c
    //void sAnmInfos_Rod_Signal; // 0x803a3bd8
    //void sAnmInfos_Rod_PukuS; // 0x803a3c14
    //void sAnmInfos_Rod_PukuL; // 0x803a3c50
    //void sAnmStateTable; // 0x803a3c8c
    //void sDemoAnmStateTable; // 0x803a3cac
    // Inline/Unused
    //void getPointPos(JGeometry::TVec3f *);
    u8 _150[0x190 - 0x150];
    TJugemRodSignal *mSignal; // 190
    u8 _194[0x1a8 - 0x194]; //
}; // Size: 0x1a8

#endif
