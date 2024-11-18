#include <JSystem/JUtility/JUTAssert.h>
#include <JSystem/JSupport.h>
#include "JSystem/JGeometry/Util.h"
#include "Kaneshige/KartMtx.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Osako/system.h"
#include "Shiraiwa/SiUtil.h"
#include "Sato/ObjUtility.h"
#include "Yamamoto/kartCtrl.h"
#include "Yamamoto/kartCamera.h"
#include "dolphin/mtx.h"
#include "macros.h"

bool SiUtil::checkPassFrame(f32 x, f32 y, f32 z) {
    if (z == 0.0f)
        return false;
    
    bool ret = false;
    const f32 diff = x-y;    
    
    if (z > 0.0f) {
        if (x-y >= 0.0f && diff <= z) {
            ret = true;
        }
    } 
    else {
        if (x-y <= 0.0f && diff >= z) {
            ret = true;
        }
    }
    return ret;
}
void SiUtil::getViewport(JGeometry::TBox2f *vp, u8 pos, u8 consoleNo) {
    switch(consoleNo) {
    case 1:
        vp->i.x = System::get3DVpX();
        vp->i.y = System::get3DVpY();
        vp->f.x = vp->i.x+System::get3DVpW();
        vp->f.y = vp->i.y+System::get3DVpH();
        break;
    case 2:
        vp->i.x = System::get3DVpDiv2X((u8)pos);
        vp->i.y = System::get3DVpDiv2Y((u8)pos);
        vp->f.x = vp->i.x+System::get3DVpDiv2W((u8)pos);
        vp->f.y = vp->i.y+System::get3DVpDiv2H((u8)pos);
        break;     
    case 3:
    case 4:
        vp->i.x = System::get3DVpDiv4X((u8)pos);
        vp->i.y = System::get3DVpDiv4Y((u8)pos);
        vp->f.x = vp->i.x+System::get3DVpDiv4W((u8)pos);
        vp->f.y = vp->i.y+System::get3DVpDiv4H((u8)pos);
        break;
    default:
#line 84
        JUT_ASSERT_MSG(0, "illegal console Number.")
        break;
    }
}

void SiUtil::getViewport(JGeometry::TBox2f *vp, u8 idx) {
    KartCam *kartCam = GetKartCtrl()->getKartCam(idx);
    vp->i.x = kartCam->GetPosv();
    vp->i.y = kartCam->GetPosh();
    vp->f.x = kartCam->GetPosv()+kartCam->GetWidth();
    vp->f.y = kartCam->GetPosh()+kartCam->GetHeight();
    //vp->set(kartCam->GetPosv(), kartCam->GetPosh(), kartCam->GetPosv()+kartCam->GetWidth(), kartCam->GetPosh()+kartCam->GetHeight());
}

int SiUtil::getScreenPosCheckMirror(int camNo, const JGeometry::TVec3f &worldPos, JGeometry::TVec2f *screenPos) {
    return getScreenPos(camNo, worldPos, screenPos, RCMGetManager() ? RCMGetManager()->isMirror() : false);
}

int SiUtil::getScreenPos(int camNo, const JGeometry::TVec3f &worldPos, JGeometry::TVec2f *screenPos, bool mirror) {
    KartCam *kartCam = RCMGetCamera((int)camNo);
    Mtx44 m;
    KartMtxPerspective(m, kartCam->GetFovy(), kartCam->GetAspect(), kartCam->GetNear(), kartCam->GetFar(), mirror);
    Vec v;
    PSMTXMultVec(kartCam->GetMtx(), &worldPos, &v);

    if (v.z >= 0.0f) {
        return -1;
    }

    PSMTX44MultVec(m, &v, &v);
    int ret = -1;
    if (v.z <= 0.0f && v.z >= -1.0f) {
        ret = (u32)(16777215.0f * (1.0f + v.z));
        screenPos->set(
            0.5f * ((1.0f + v.x) * kartCam->GetVpWidth()) + kartCam->GetVpPosv(),
            0.5f * ((1.0f - v.y) * kartCam->GetVpHeight()) + kartCam->GetVpPosh());
    }

    return ret;
}

void SiUtil::setItemReaction(const void *head, GeographyObj *geoObj) {
#line 196
    JUT_ASSERT(head != 0);

    JSUMemoryInputStream stream(head, *(s32*)head);

    stream.skip(4);
    
    u16 val1, val2;
    // I think these are supposed to use the >> operators however those doesn't seem to work or i didn't implement it correctly
    // This template approach at least seems to work and i also think it's cleaner
    u16 endJ = stream.read(&val1);
    u16 endI = stream.read(&val2);
    u32 numreads = ((0.5f * endJ) + 2.0f);
    u32 pos = numreads - 2;
    
    for (int i = 0; i < endI; i++) {
        u16 kind;
        stream.read(&kind);
        if (kind == geoObj->getKind()) {
            u8 prev;
            u8 cur;
            for (int j = 0; j < endJ; j++) {                
                if (!(j & 1)) {
                    u8 val3;
                    prev = stream.read(&val3);
                    cur = JSUHiNibble(prev);                    
                }
                else {
                    cur = JSULoNibble(prev);
                }
                geoObj->getItemReaction().setFlg((u8)j, cur);
            }
            break;
        }
        stream.skip(pos);
    }
}

void SiUtil::clampVec(const JGeometry::TVec3f &cur, JGeometry::TVec3f &out, const JGeometry::TVec3f &low, const JGeometry::TVec3f &high) {
    f32 lowX = low.x;
    f32 highX = high.x;
    out.x = CLAMP(lowX, highX, cur.x);
    
    f32 lowY = low.y;
    f32 highY = high.y;   
    out.y = CLAMP(lowY, highY, cur.y);

    f32 lowZ = low.z;
    f32 highZ = high.z;
    out.z = CLAMP(lowZ, highZ, cur.z);
}

f32 SiUtil::getNormalRange(f32 cur, f32 min, f32 max) {
    f32 val = cur > max ? max : (cur < min) ? min : cur;
    return (val - min) / (max - min);
}

u8 SiUtil::getConsoleNum() {
#line 354
    int ret = RCMGetManager()->getConsoleNumber();
    JUT_MINMAX_ASSERT(1, ret, 5);
    return ret;
}

u8 SiUtil::getDrawConsoleNum() {
#line 372
    int ret = RCMGetManager()->getCurrentConsoleNumber();
    JUT_MINMAX_ASSERT(1, ret, 5);
    return ret;
}

f32 SiUtil::getCourseHeight(JGeometry::TVec3f &pos) {
    CrsGround ground(RCMGetCourse());
    ground.search(pos);
    return ground.getHeight();
}

f32 SiUtil::getWaterHeight(JGeometry::TVec3f &pos) {
    CrsGround ground(RCMGetCourse());
    ground.search(pos);
    return ground.getWaterHeight();
}

s16 SiUtil::searchNearKartBall(const JGeometry::TVec3f &enemyPos, f32 r) {
    f32 rad = r*r;
    s16 ret = -1;
    for (s16 i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        JGeometry::TVec3f posDiff;
        ObjUtility::getKartPos(i, &posDiff);
        posDiff.sub(enemyPos);
        f32 len = posDiff.squared();
        if (len < rad) {
            rad = len;
            ret = i;            
        }
    }
    return ret;
}

s16 SiUtil::searchNearKartCircle(const JGeometry::TVec3f &enemyPos, f32 r) {
    f32 rad = r*r;
    s16 ret = -1;
    for (s16 i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        JGeometry::TVec3f posDiff;
        ObjUtility::getKartPos(i, &posDiff);
        posDiff.set(posDiff.x - enemyPos.x, 0.0f, posDiff.z - enemyPos.z);
         
        f32 len = posDiff.squaredZX();
        if (len < rad) {
            rad = len;
            ret = i;            
        }
    }
    return ret;
}

s16 SiUtil::setRandomStartFrame(J3DFrameCtrl *frameCtrl, stRandom *rand) {
    s16 frame = rand->getRandomMax(frameCtrl->getEnd());
    frameCtrl->setFrame(frame);
    return frame;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
