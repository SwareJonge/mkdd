#include "JSystem/JGeometry/Vec.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Course/CrsData.h"

#include "Sato/stMath.h"
#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

const u32 CrsData::cPatchCamTagTable[] ={
    'prma', 'debu', 
    'spec', 'loca'
};

const s16 CrsData::cPatchCamTableNtscE[] ={
    600, 600, 500, 550
};

const s16 CrsData::cPatchCamTablePal[] ={
    600, 600, 450, 600
};

CrsData::CrsData(CrsData::SColHeader *bco, CrsData::SOblHeader *bol) {
    mCollionData = bco;
    mObjectData = bol;
    if (bco) {
#line 439
        JUT_ASSERT_F(bco->version == '0003', "BCO INVAL VER.%08x ORG[%08x]", bco->version, '0003');
    }
    JUT_ASSERT_F(bol->version == '0015', "BOL INVAL VER.%08x ORG[%08x]", bol->version, '0015');

    if (isAvailableCol()) {
        mMatData = new SMatData[getMatNum()];

        for(int i = 0; i < getMatNum(); i++) {
            SMatData *matData = &mMatData[i];
            SMaterial *mat = getMatTable();
            const char *matName = "UNKNOWN";

            switch (mat[i].colFlag)
            {
            case 0:
                matName = "DIRT";
                break;
            case 1:
                matName = "ROAD";
                break;
            case 2:
                matName = "WALL";
                break;
            case 3:
                matName = /*TOUCH_*/"GRASS";
                break;
            case 4:
                matName = "ICE";
                break;
            case 5:
                matName = "RESQ";
                break;
            case 14:
                matName = "RESQD";
                break;
            case 15:
                matName = "RESQS";
                break;
            case 6:
                matName = "SLIP";
                break;
            case 7:
                matName = "DASHJ"; 
                break;
            case 8:
                matName = "DASHB";
                break;
            case 9:
                matName = "CANNON"; 
                break;
            case 10:
                matName = "VALLEY";
                break;
            case 17:
                matName = "BEACH";
                break;
            case 18:
                matName = "OUTF";
                break;
            case 255:
                matName ="NONE";
                break;
            }

            snprintf(matData->name, 0x10, "☆%s%d", matName, mat[i].soundID);
        }
    }
    else {
        mMatData = nullptr;
    }
}

void CrsData::patchCamDataForStaffRoll(Language lang, VideoMode vMode) {
    const s16 *camTable = nullptr;
    
    switch (vMode)
    {
    case VideoMode_NTSC:
        if (lang == ENGLISH)
            camTable = cPatchCamTableNtscE;
        break;    
    case VideoMode_PAL:
        camTable = cPatchCamTablePal;
        break;
    }

    if (camTable) {
        for (int i = 0; i < 4; i++) {
            int cameraID = searchCameraID(cPatchCamTagTable[i]);
            if (cameraID >= 0) {
                Camera *camera = getCameraData(cameraID);
                camera->mTime = camTable[i];
            }
        }
    }
}

// Really feel like this code doesn't make sense, try to rewrite
CrsData::SMaterial *CrsData::searchSameAttr(u8 attr, u8 attrIdx) const {
    SMaterial *ret = nullptr;
    bool same = false;
    SMaterial *matTable = getMatTable();
    for (int i = 0; i < getMatNum(); i++) {
        SMaterial *mat = &matTable[i];

        if (!same) {
            if (mat->colFlag != attr)
                continue;            
            same = true;
        }
        else if (mat->colFlag != attr)
            break;

        if(mat->soundID == attrIdx) {
            ret = mat;
            break;
        }        
    }
    return ret;
}

u16 CrsData::searchMaterial(u8 attr, u8 attrIdx) const {
    u16 ret = 0xff;
    SMaterial *mat = searchSameAttr(attr, attrIdx);
    if (mat)
        ret = mat->_2;
    
    return ret;
}

void CrsData::getLightColor(JUtility::TColor *color) const {
    if (isAvailableObj())
        color->set(mObjectData->lightColor.r, mObjectData->lightColor.g, mObjectData->lightColor.b, mObjectData->lightColor.a);
    else
        color->set(0xff,0xff,0xff,0xff);
}

bool CrsData::isAvailableObj() const { return mObjectData != nullptr; }

void CrsData::getLightOffsetPosition(JGeometry::TVec3f *pos) const {
    if (isAvailableObj())
        pos->set(mObjectData->lightOffsetPos.x, mObjectData->lightOffsetPos.y, mObjectData->lightOffsetPos.z);
    else
        pos->zero();
}

void CrsData::getAmbientColor(JUtility::TColor *color) const {
    if (isAvailableObj())
        color->set(mObjectData->ambientColor[0], mObjectData->ambientColor[1], mObjectData->ambientColor[2], 0);
    else
        color->set(0x64,0x64,0x64,0);
}

void CrsData::getShadowColor(JUtility::TColor *color) const {
    if (isAvailableObj())
        color->set(mObjectData->shadowColor[0], mObjectData->shadowColor[1], mObjectData->shadowColor[2], 0x50);
    else
        color->set(0,0,0x14,0x50);
}

int CrsData::getFogType() const {
    int fogType = 0;

    if (isAvailableObj()) {
        switch (mObjectData->fogType)
        {
        case 0:
            fogType = 2;
            break;
        case 1:
            fogType = 4;
            break;
        case 2:
            fogType = 5;
            break;
        case 3:
            fogType = 6;
            break;
        case 4:
            fogType = 7;
            break;
        }
        // fogType = mObjectData->fogType == 0 ? 2 : mObjectData->fogType + 3;
    }
    return fogType;
}

f32 CrsData::getFogStartZ() const {
    f32 z = 8000.0f;

    if (isAvailableObj())
        z =mObjectData->fogStartZ;
    return z;
}

f32 CrsData::getFogEndZ() const {
    f32 z = 8000.0f;

    if (isAvailableObj())
        z =mObjectData->fogEndZ;
    return z;
}

void CrsData::getFogColor(JUtility::TColor *color) const {
    if (isAvailableObj())
        color->set(mObjectData->fogColor[0], mObjectData->fogColor[1], mObjectData->fogColor[2], 0);
    else
        color->set(0xff,0xff,0xff,0);
}

CrsData::StartPoint* CrsData::getStartPoint(u8 startID) const {
    StartPoint *startPoint = nullptr;
    int startPointCnt = getStartPointNumber(); 
    u8 playerID = startID;

    if (startPointCnt == 0 && playerID == 0xff)
        playerID = 0;
    
    if (startPointCnt == 0)
        startPointCnt = 1;
    
    for (int i = 0; i < startPointCnt; i++) {
        StartPoint *startPointTbl = getStartPointTable();
        StartPoint *curPoint = &startPointTbl[i];
        if (curPoint->mPlayerID == playerID) {
            startPoint = curPoint;
            break;
        }           
    }
    return startPoint;
}

CrsData::Ground *CrsData::Grid::searchGround(stPlaneParam *plane, const JGeometry::TVec3f &worldPos, const JGeometry::TVec3f&p3, const CrsData::Ground *pGround, const u16 *indexTable, const CrsData::SGround *groundTable, const JGeometry::TVec3f *vertexTable, u32 p8, bool exceptValley) {
    Ground *foundGround = nullptr;
    const u16 *pIndex = &indexTable[mGroundIndex];

    for (int i = 0; i < mCount; i++, pIndex++) {
        Ground *ground = (Ground *)&groundTable[*pIndex];

        if (!exceptValley || ground->attribute != 0xa) {
            switch (p8) { // p8 should always be 3
            case 1:
                if (ground->attribute == 2)
                    continue;
                break;
            case 2:
                if (ground->attribute != 2)
                    continue;
                break; 
            }
            if (ground->checkPosition(plane, worldPos, p3, vertexTable)) {
                foundGround = ground;
                break;
            }
        }
    }
    return foundGround;
}

CrsData::Ground *CrsData::Grid::checkPolygonBySphere(JGeometry::TVec3f *p1, f32 *pScale, const CrsData *crsData, const CrsData::SColInfoSphere &infoSphere, f32 *p5) {
    CrsData::Ground *ground = nullptr;
    u16 *indexTable = crsData->getIndexTable();
    SGround *groundTable = crsData->getGroundTable();
    u16 *gndIdxTbl = indexTable + mGroundIndex;

    JGeometry::TVec3f v1;
    JGeometry::TVec3f v2;
    v2.zero();

    f32 f = 0.0f;

    for (int i = 0; i < mCount; i++, gndIdxTbl++) {
        Ground *groundPoint = (Ground *)&groundTable[*gndIdxTbl];
        switch (groundPoint->attribute) {
        case 0x12:    
        case 0x2: {
            JGeometry::TVec3f v3;
            JGeometry::TVec3f v4;
            f32 scale = groundPoint->checkWallBySphere(&v1, infoSphere, crsData->getVertexTable(), &f, &v3, &v4);
            bool noCover = false; 
            f32 f2 = -1.0f;
            if (scale >= 0.0f) {
                CrsGround grnd(RCMGetCourse());
                if (!grnd.checkPolygonCover(v3, v4, infoSphere._0, infoSphere.d, groundPoint, &f2))
                    noCover = true;
            }
            if (noCover) {
                if (ground == nullptr) {
                    *p5 = f2;
                    ground = groundPoint;
                }
                v2.scaleAdd(scale, v1, v2);
                if (*pScale < scale) {
                    *pScale = scale;
                }
            }            
        }
        }
    }
    
    if (*pScale >= 0.0f) {
        v2.normalize();
        p1->set(v2);
    }
    else {
        ground = nullptr;
    }

    return ground;
}

bool CrsData::Ground::checkPosition(stPlaneParam *plane, const JGeometry::TVec3f &worldPos, const JGeometry::TVec3f &p3, const JGeometry::TVec3f *vertexTable) const {
    bool ret = false;
    Course *course = RaceMgr::getManager()->getCourse();
    f32 addThickness = course->getGroundThickness();

    if (getAddThickness() != 0)
        addThickness += getAddThickness() * 100.0f;

    int xminId = getXminIndex();
    int zminId = getZminIndex();
    int xmaxId = getXmaxIndex();
    int zmaxId = getZmaxIndex();

#line 1078
    JUT_MINMAX_ASSERT(0, xminId, 3)
    JUT_MINMAX_ASSERT(0, zminId, 3)
    JUT_MINMAX_ASSERT(0, xmaxId, 3)
    JUT_MINMAX_ASSERT(0, zmaxId, 3)

    JGeometry::TVec3f aWorldPos[3];

    for(int i = 0; i < 3; i++)
        course->convMapCoordToWorldPos(&aWorldPos[i], vertexTable[pointIndices[i]]);

    if (worldPos.x < aWorldPos[xminId].x)
        return false;
    if (worldPos.x > aWorldPos[xmaxId].x)
        return false;
    if (worldPos.z < aWorldPos[zminId].z)
        return false;
    if (worldPos.z > aWorldPos[zmaxId].z)
        return false;

    stPlaneParam newPlane;
    course->calcPlaneParam(&newPlane, normDir.x * 0.0001f, normDir.y * 0.0001f, normDir.z * 0.0001f, dir, aWorldPos[0]);

    //f32 n = ((newPlane.x * worldPos.x)+(newPlane.y * worldPos.y)+(newPlane.z * worldPos.z) + newPlane.d);
    if (((newPlane.x * worldPos.x)+(newPlane.y * worldPos.y)+(newPlane.z * worldPos.z) + newPlane.d) < -0.0f) {
        f32 a = -((newPlane.x * worldPos.x)+(newPlane.z * worldPos.z) + newPlane.d) / newPlane.y;
        if (a - worldPos.y > addThickness) {
            return false;
        }
    }
    
    u8 attr = attribute;
    for (int i = 0; i < 3; i++) {
        const JGeometry::TVec3f &cur = aWorldPos[i];
        JGeometry::TVec3f diff;

        diff.sub(aWorldPos[(i + 1) % 3], cur);
        diff.normalize();
        
        JGeometry::TVec3f worldPosDiff;
        worldPosDiff.sub(worldPos, cur);
        worldPosDiff.normalize();

        f32 len = diff.z * worldPosDiff.x - diff.x * worldPosDiff.z;

        bool valid = true;

        switch (attr)
        {
        case 0x02:
        case 0x12:
            if (len <= 0.0f)
                valid = false;
            break;
        
        default:
            if (len < -0.01f)
                valid = false;
            break;
        }

        if (!valid)
            break;

        if (i == 2) {
            ret = true;
            break;
        }
    }

    if (ret) {
        *plane = newPlane;
    }

    return ret;
}

f32 CrsData::Ground::checkWallBySphere(JGeometry::TVec3f *p1, const CrsData::SColInfoSphere &infoSphere, const JGeometry::TVec3f *vertexTbl, f32 *pY, JGeometry::TVec3f *p5, JGeometry::TVec3f *p6) const {
    Course *course = RaceMgr::getManager()->getCourse();
    SGround *gnd = course->getCrsData()->getGroundTable();
    f32 thickness = course->getGroundThickness();

    JGeometry::TVec3f aWorldPos[3];

    for (int i = 0; i < 3; i++) {
        course->convMapCoordToWorldPos(&aWorldPos[i], vertexTbl[pointIndices[i]]);
    }
    f32 ret = -1.0;
    *pY = getPlaneY(infoSphere._0, course);

    if (*pY < (infoSphere._0.y + infoSphere.d)) {
        for (int i = 0; i < 3; i++) {
            bool sameFlag = false;
            int pos2Idx = (i+1) % 3;
            u16 trig = nearTrigs[i];
            
            if (trig == 0xffff || attribute != gnd[trig].attribute) {
                sameFlag = true;
            }
            if (sameFlag) {
                f32 f1 = -1.0;
                JGeometry::TVec3f v1;
                u32 touchState = getTouchState(infoSphere, aWorldPos[i], aWorldPos[pos2Idx], &v1, &f1, 0.0f);
                bool touching = false;
                f32 f2 = infoSphere.d - f1;
                switch (touchState) {
                case 0: {
                    ret = -1.0f;
                    sameFlag = false;
                    p1->zero();
                    break;
                }
                case 1: {
                    sameFlag = true;
                    touching = true;
                    break;
                }
                case 2: {
                    if (f2 < ret) {
                        sameFlag = true;
                    }
                    else {
                        sameFlag = false;
                    }
                    touching = false;
                    break;
                }
                case 3: {
                    sameFlag = false;
                    touching = false;
                    break;
                }
                }
                if (sameFlag && f2 > ret) {
                    ret = f2;
                    p1->set(v1);
                    getNearPoint(infoSphere, f2, v1, aWorldPos[i], aWorldPos[pos2Idx], p5, p6);
                }
                if (touching) break;
            }
        }
    }
    return ret;
}

void CrsData::Ground::getNearPoint(const CrsData::SColInfoSphere &infoSphere, f32 d, const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2, const JGeometry::TVec3f &pos3, JGeometry::TVec3f *p6, JGeometry::TVec3f *p7) const {
    Course *course = RCMGetCourse();
    const CrsData *crsData = course->getCrsData();
    JGeometry::TVec3f *vertexTbl = crsData->getVertexTable();
    JGeometry::TVec3f dir(infoSphere._0);
    f32 sphereDir = infoSphere.d;

    JGeometry::TVec3f v1;
    v1.sub(pos3, pos2);

    JGeometry::TVec3f v2;
    JGeometry::TVec3f v3;
    v2.sub(dir, pos2);
    v3.sub(dir, pos3);

    JGeometry::TVec3f center;
    getCenter(&center,vertexTbl,course);

    f32 v2ZXdp = v2.dotZX(v1);
    f32 v3ZXdp = v3.dotZX(v1);
    JGeometry::TVec3f v4;
    f32 scalar;
    if (v2ZXdp < 0.0f) {
        v4.sub(center, pos2);
        v4.normalize();
        dir.set(pos2);
        scalar = 0.0f;
    }
    else if (v3ZXdp > 0.0f) {
        v4.sub(center, pos3);
        v4.normalize();
        dir.set(pos3);
        scalar = 0.0f;
    }
    else {
        v4.negate(pos1);
        v4.normalize();
        scalar = (sphereDir - d);
    }

    static const f32 inTickness[4] ={
        5.0f, 1.0f, 0.5f, 0.01f
    };

    for (int i = 0; i < 4; i++) {
        p6->scaleAdd(scalar + inTickness[i], v4, dir);
        u32 touchState = checkPolygonTouchBySphere(vertexTbl, *p6, sphereDir, course);
        if (touchState == 1) continue;
        if (touchState != 0) break;
    }
    p7->scaleAdd(scalar - 1.0f, v4, dir);
}

f32 CrsData::Grid::checkPolygonCoverWall(const JGeometry::TVec3f &pos, const CrsData::SColInfoSphere &infoSphere, const CrsData::Ground *ground) {
    JGeometry::TVec3f v1(infoSphere._0);
    v1.y += CrsGround::getOverLevel();
    f32 d = infoSphere.d;
    const CrsData *crsData = RCMGetCourse()->getCrsData();

    u16 *indexTable = crsData->getIndexTable();
    Ground *groundTable = crsData->getGroundTable();
    f32 ret = -1.0f;
    
    u16 *gndIdxTbl = indexTable + mGroundIndex;
    
    for (int i = 0; i < mCount; i++, gndIdxTbl++) {
        const Ground *groundPoint = &groundTable[*gndIdxTbl];
        Course *course = RaceMgr::getManager()->getCourse();
        f32 planeY = groundPoint->getPlaneY(pos, course);
        JGeometry::TVec3f v2;
        v2.set(pos);
        v2.y = planeY;

        if (groundPoint != ground && groundPoint->checkFront(crsData, v1) && (ground == nullptr || ground->checkFaceSide(crsData, v2))) {
            u32 touchState = groundPoint->checkPolygonTouchBySphere(crsData->getVertexTable(), pos, d, course);
            if (touchState == 2 || touchState == 3) {
                switch (groundPoint->attribute) {
                case 0x12:    
                case 0x2:
                    groundPoint = ground;
                    break;
                default:
                    ret = planeY;
                    break;
                }
                
            }
        }
        if (groundPoint == ground) break;
        if (ret != -1.0f) break;
    }

    return ret;
}

bool CrsData::Ground::checkFront(const CrsData *crsData, const JGeometry::TVec3f &p) const {
    bool isFront = true;
    Course *course = RaceMgr::getManager()->getCourse();

    if (!checkFaceSide(crsData, p)) {
        JGeometry::TVec3f *vertexTable = crsData->getVertexTable();
        JGeometry::TVec3f worldPos;
        for (int i = 0; i < 3; i++) {
            course->convMapCoordToWorldPos(&worldPos, vertexTable[pointIndices[i]]);
            if (p.y < worldPos.y) {
                isFront = false;
                break;
            }
        }
    }

    return isFront;
}

bool CrsData::Ground::checkFaceSide(const CrsData *crsData, const JGeometry::TVec3f &p) const {
    stPlaneParam plane;
    Course *course =  RaceMgr::getManager()->getCourse();
    course->calcPlaneParam(&plane, normDir.x * 0.0001f, normDir.y * 0.0001f, normDir.z * 0.0001f, dir, p);

    JGeometry::TVec3f dir;
    dir.x = plane.x;
    dir.y = plane.y;
    dir.z = plane.z;
    dir.normalize();

    
    JGeometry::TVec3f *vertexTable = crsData->getVertexTable();
    JGeometry::TVec3f worldPos;
    course->convMapCoordToWorldPos(&worldPos, vertexTable[pointIndices[0]]);
    JGeometry::TVec3f diff;
    diff.sub(p, worldPos);
    diff.normalize();

    return diff.dot(dir) >= 0.0f;
}

f32 CrsData::Ground::getPlaneY(const JGeometry::TVec3f &p, Course *course) const {
    stPlaneParam n;

    if (course) {
        JGeometry::TVec3f up;
        up.set(p.x, dir, p.z);

        course->calcPlaneParam(&n, normDir.x * 0.0001f, normDir.y * 0.0001f, normDir.z * 0.0001f, dir, up);
    }
    else {
        n.x = normDir.x * 0.0001f;
        n.y = normDir.y * 0.0001f;
        n.z = normDir.z * 0.0001f;
        n.d = dir;
    }

    return -(n.x * p.x + n.z * p.z + dir) / n.y;
}

u32 CrsData::Ground::checkPolygonTouchBySphere(const JGeometry::TVec3f*vertexTable, const JGeometry::TVec3f &p, f32 d, Course *course) const {
    JGeometry::TVec3f aWorldPos[3];

    for(int i = 0; i < 3; i++)
        course->convMapCoordToWorldPos(&aWorldPos[i], vertexTable[pointIndices[i]]);
    
    bool isNot2 = true;
    SColInfoSphere sphere;
    sphere._0.set(p);
    sphere.d = d;

    for (int i = 0; i < 3; i++) {
        u32 touchState = getTouchState(sphere, aWorldPos[i], aWorldPos[(i+1) % 3], nullptr, nullptr, 0.0f);

        if (touchState == 1 || touchState == 0)
            return touchState;
        else if(touchState == 2)
            isNot2 = false;
    }

    int ret = 2;
    if (isNot2)
        ret = 3;
    return ret;
    
}

u32 CrsData::Ground::getTouchState(const CrsData::SColInfoSphere &infoSphere, const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2, JGeometry::TVec3f *p4, f32 *p5, f32 p6) const {
    if (p4) {
        p4->zero();
    }
    if (p5) {
        *p5 = -1.0f;
    }

    JGeometry::TVec3f sphereDir(infoSphere._0);
    f32 d = infoSphere.d;
    JGeometry::TVec3f v1;
    v1.sub(pos2, pos1);
    JGeometry::TVec3f yDir(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f v2;
    v2.cross(v1, yDir);
    v2.normalize();
    JGeometry::TVec3f v3;
    v3.sub(sphereDir, pos1);
    JGeometry::TVec3f v4;
    v4.sub(sphereDir, pos2);
    f32 v3v2dp = v3.dot(v2);

    if (v3v2dp > d) 
        return 0;

    JGeometry::TVec3f v5;
    v5.cross(yDir, v2);
    v5.normalize();
    f32 v3v5dp = v3.dot(v5);
    f32 v4v5dp = v4.dot(v5);
    if ((v3v2dp > 0.0f) && (v3v5dp < -d))
        return 0;

    if ((v3v2dp > 0.0f) && (v4v5dp > d))
        return 0;

    if (v3v2dp <= 0.0f ) {
        if (p5) {
            *p5 = (float)v3v2dp;
        }
        if (v3v2dp < -(d + p6)) {
            return 3;
        }
        
        if (p4) {
            p4->set(v2);
        }
        return 2;
        
    }

    f32 len = (d * d);
    if ((v4v5dp > 0.0f) && (v3v2dp > 0.0f)) {
        if (v4.squaredZX() > len) {
            return 0;
        }
        v4.y = 0.0f;
        v3v2dp = v4.length();
    }
    else if ((v3v5dp < 0.0f) && (v3v2dp > 0.0f)) {
        if (v3.squaredZX() > len) {
            return 0;
        }
        v3.y = 0.0f;
        v3v2dp = v3.length();
    }

    if (p4) {
        p4->set(v2);
    }
    if (p5) {
        *p5 = (float)v3v2dp;
    }
    return 1;
}

void CrsData::Ground::getCenter(JGeometry::TVec3f *center, const JGeometry::TVec3f *vertexTable, Course *course) const {
    JGeometry::TVec3f aWorldPos[3];

    for(int i = 0; i < 3; i++)
        course->convMapCoordToWorldPos(&aWorldPos[i], vertexTable[pointIndices[i]]);
    
    for(int i = 0; i < 3; i++)
        center->add(aWorldPos[i]);

    center->scale(0.33333334f);
}

CrsData::SCheckPoint *CrsData::getCheckPointTable() const {
    CheckPath *checkPathTbl = getCheckPathTable();
    return (SCheckPoint *)(checkPathTbl + getCheckPathNumber()); // CheckPoint Data is after the CheckPath section
}

int CrsData::getCheckPointOffset(int n) const {
    int offset = 0;
    CheckPath *pathTable = getCheckPathTable();

    for (int i = 0; i < n; i++)
        offset += pathTable[i].pointCount;
    return offset;
}

bool CrsData::SObject::isAvailableObject() {
    bool ret = false;
    
    int filterType = -1;
    switch(RaceMgr::getManager()->getRaceMode()) {
        case BALLOON_BATTLE:
            filterType = 0;
            break;            
        case ROBBERY_BATTLE:
            filterType = 1;
            switch (objID)
            {
            case 1:
            case 0x26b2:
            {
                s16 maxKarts = mParam8;
                if (maxKarts > 1 && RaceMgr::getManager()->getKartNumber() <= maxKarts)
                    filterType = -1;
                break;
            }          
            default:
                break;
            }
            break;        
        case BOMB_BATTLE:
            filterType = 2;
            break;
        case ESCAPE_BATTLE:
            filterType = 3;
            break;
        case TIME_ATTACK:
            filterType = 7;
            break;
        default:
            ret = true;
            break;
    }

    if (filterType >= 0)
        ret = (presence.filter & 1 << filterType) != 0;

    if (ret) {
        ret = isSameProcLevel(RaceMgr::getManager()->getProcLevel());
    }
    return ret;
}

bool CrsData::SObject::isSameProcLevel(int level) {
    bool ret = false;
    if (level >= 0 && level <= 7)
        ret = (presence.flag & 1 << level) != 0;
    return ret;
}

int CrsData::CheckPath::getELinkNum() const {
    int i = 0;
    for(; i < 4; i++) {
        if (next[i] < 0) {
            break;
        }
    }
    return i;
}

void CrsData::StartPoint::getLinePosition(JGeometry::TVec3f *linePos) const {
    getPosition(linePos);
    JGeometry::TVec3f dir;
    getFrDirection(&dir);
    dir.scale(500.0f);
    linePos->add(dir);
}

int CrsData::getStartCameraID() const {
    int cameraID = -1;
    for (int i = 0; i < getCameraNumber(); i++) {
        Camera *camera = getCameraData(i);
        if (camera->isStartCamera()){
            cameraID = i;
            break;
        }
    }
    return cameraID;
}

//void CrsData::getCameraNumber() const {}

//void CrsData::Camera::isStartCamera() const {}

int CrsData::getStartLoserCameraID() const {
    return searchCameraID('lose');
}

int CrsData::searchCameraID(u32 camTag) const {
    int cameraID = -1;
    for (int i = 0; i < getCameraNumber(); i++) {
        Camera *camera = getCameraData(i);
        if(camera->mName == camTag) {
            cameraID = i;
            break;
        }
           
    }
    return cameraID;
}

CrsData::Camera *CrsData::getCameraData(int camIdx) const {
    Camera *camData = nullptr;

    if (camIdx >= 0 && camIdx < getCameraNumber())
        camData = reinterpret_cast<Camera *>(reinterpret_cast<u8 *>(mObjectData) + mObjectData->cameraOffset) + camIdx;
    return camData;
}

void CrsData::Camera::getFrontDirection(JGeometry::TVec3f *dir) const {
    dir->set(mDir);
    dir->scale(0.0001f);
}

CrsData::PathData *CrsData::getPathData(u16 pathIdx) const {
    PathData *pathData = nullptr;

    if (pathIdx < getPathNumber())
        pathData = &getPathTable()[pathIdx];
    return pathData;
}


CrsData::PointData *CrsData::getPointData(u16 pathIdx, u16 pointIdx) const {
    PointData *pointData = nullptr;
    PathData *pathData = getPathData(pathIdx);

    if (pointIdx < pathData->pointCount) {
        PointData *start = &getPointTable()[pathData->pointStart];
        pointData = &start[pointIdx];
    }
    return pointData;
}

const char *CrsData::getMatTableLabel(int matNo) const { 
    // UNUSED, fabricated
    // Size mismatch, should be 0xa4, currently is 0xa8
    JUT_MINMAX_ASSERT(0, matNo, getMatNum());
    return mMatData[matNo].name;
}

CrsData::MGParam *CrsData::getMGParam(int paramNo) const {
    MGParam *mgParams = nullptr;

    if(mObjectData->mgParamOffset != 0) {
        MGParam *paramTbl = getMGParamTable();
        int paramIdx = paramNo - 1;
        if (paramIdx >= 0 && paramIdx < getMGParamNumber())
            mgParams = &paramTbl[paramIdx];
    }
    return mgParams;
}

CrsData::LightParam *CrsData::getLightParam(s16 lightNo) const {
    LightParam *lightParam = nullptr;
    int numLights = getLightParamNumber();

    if (lightNo >= 0 && lightNo < numLights)
        lightParam = &getLightParamTable()[lightNo];
    return lightParam;
}
