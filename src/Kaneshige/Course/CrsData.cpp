#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Course/CrsData.h"

#include "mathHelper.h"

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

            snprintf(matData->name, 0x10, "â˜?%s%d", matName, mat[i].soundID);
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
CrsData::SMaterial *CrsData::searchSameAttr(unsigned char attr, unsigned char attrIdx) const {
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

u16 CrsData::searchMaterial(unsigned char attr, unsigned char attrIdx) const {
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

CrsData::Ground *CrsData::Grid::checkPolygonBySphere(JGeometry::TVec3f*, float *, const CrsData *crsData, const CrsData::SColInfoSphere &infoSphere, float *) {
    CrsData::Ground *ground = nullptr;
    u16 *indexTable = crsData->getIndexTable();
    SGround *groundTable = crsData->getGroundTable();

    JGeometry::TVec3f v1;
    JGeometry::TVec3f v2;
    v2.zero();

    for (int i = 0; i < mCount; i++) {

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

    // regswaps here
    if (((newPlane.x * worldPos.x)+(newPlane.y * worldPos.y)+(newPlane.z * worldPos.z) + newPlane.direction) < 0.0f && -((newPlane.x * worldPos.x)+(newPlane.z * worldPos.z) + newPlane.direction) / newPlane.y - worldPos.y > addThickness)
        return false;
    
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

f32 CrsData::Ground::checkWallBySphere(JGeometry::TVec3f*, const CrsData::SColInfoSphere &, const JGeometry::TVec3f*, float *, JGeometry::TVec3f*, JGeometry::TVec3f*) const {}

void CrsData::Ground::getNearPoint(const CrsData::SColInfoSphere &, float, const JGeometry::TVec3f&, const JGeometry::TVec3f&, const JGeometry::TVec3f&, JGeometry::TVec3f*, JGeometry::TVec3f*) const {}

f32 CrsData::Grid::checkPolygonCoverWall(const JGeometry::TVec3f&, const CrsData::SColInfoSphere &, const CrsData::Ground *) {}

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

bool CrsData::Ground::checkFaceSide(const CrsData *CrsData, const JGeometry::TVec3f &p) const {

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
        n.direction = dir;
    }

    return -(n.x * p.x + n.z * p.z + dir) / n.y;
}

int CrsData::Ground::checkPolygonTouchBySphere(const JGeometry::TVec3f*vertexTable, const JGeometry::TVec3f &p, float d, Course *course) const {
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

int CrsData::Ground::getTouchState(const CrsData::SColInfoSphere &infoSphere, const JGeometry::TVec3f&, const JGeometry::TVec3f&, JGeometry::TVec3f*, float *, float) const {}

void CrsData::Ground::getCenter(JGeometry::TVec3f *center, const JGeometry::TVec3f *vertexTable, Course *course) const {
    JGeometry::TVec3f aWorldPos[3];

    for(int i = 0; i < 3; i++)
        course->convMapCoordToWorldPos(&aWorldPos[i], vertexTable[pointIndices[i]]);
    
    for(int i = 0; i < 3; i++)
        center->add(aWorldPos[i]);

    center->scale(1/3);
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
                s16 maxKarts = robberyKarts;
                if (maxKarts > 1 && RaceMgr::getManager()->getKartNumber() <= maxKarts)
                    filterType = -1;
                break;            
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
