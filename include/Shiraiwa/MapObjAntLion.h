#ifndef MAPOBJANTLION_H
#define MAPOBJANTLION_H

#include <JSystem/J3D.h>
#include <JSystem/JKernel/JKRHeap.h>

#include "Kaneshige/Course/CrsData.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

#include "types.h"

class TMapObjAntLion : public TMapObjHioNode, public StateObserver
{
public:
    // Global
    TMapObjAntLion(const CrsData::SObject &);        // 0x802cdf80
    virtual const char *getBmdFileName();            // 0x802ce0d8
    void createColModel(J3DModelData *);             // 0x802ce100
    void loadAnimation();                            // 0x802ce184
    void createModel(JKRSolidHeap *, unsigned long); // 0x802ce1d0
    void reset();                                    // 0x802ce3dc
    void calc();                                     // 0x802ce440
    void initFunc_Start();                           // 0x802ce494
    void doFunc_Start();                             // 0x802ce4d4
    void initFunc_Wait();                            // 0x802ce4fc
    void doFunc_Wait();                              // 0x802ce5a4
    void initFunc_Eat();                             // 0x802ce5a8
    void doFunc_Eat();                               // 0x802ce748
    void getZDir(JGeometry::TVec3f *);               // 0x802ce7e4
    void doKartColCallBack(int);                     // 0x802ce884

    void eat() {
        if(getState() != 2) {
            setState(2);
        }
    }

    // void sTable;                                     // 0x803aa258
    // void scJointName;                                // 0x804154b8
    // void sAntLionBckAnmTrans;                        // 0x80417118
    // void sAntLionBckMtxCalc;                         // 0x8041711c
    // void sJointNo;                                   // 0x80417120
    //  Inline
    virtual ~TMapObjAntLion(); // 0x802ce010
    void InitExec();           // 0x802ce304
    void MoveExec();           // 0x802ce370
};

#endif // MAPOBJANTLION_H