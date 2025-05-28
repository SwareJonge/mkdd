#include <std/math.h>
#include <dolphin/gx.h>
#include <JSystem/J3D/J3DModelLoader.h>
#include <JSystem/J3D/J3DPE.h>
#include <JSystem/J3D/J3DSys.h>
#include <JSystem/J3D/J3DTexMtx.h>
#include <JSystem/J3D/J3DTransform.h>
#include <JSystem/J3D/J3DTypes.h>

#include <JSystem/JAudio/JASFakeMatch2.h>

#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/SimpleDrawer.h"
#include "Kaneshige/TexLODControl.h"
#include "Kaneshige/Course/Course.h"
#include "Yamamoto/kartCamera.h"
#include "dolphin/mtx.h"

u16 Course::sDrawAreaModel;
bool Course::sForceDebugFogSetting;
bool Course::sConfigFog;
static u8 sShakeDir;
s16 Course::sShakeRippleTime;

bool Course::sAddShakeVel = true;
f32 Course::sBoundsScale = 1.0f;
f32 Course::sGndThickness = 100.0f;
f32 Course::sShakeMaxAngle = 5.0f;
s16 Course::sShakeFirstRippleTime = 610;
s16 Course::sShakeRoughTime = 235;
s16 Course::sShakeKeepTime = 70;
s16 Course::sShakeRecoverTime = 210;
s16 Course::sShakeFirstRippleTimeDemo = 150;
f32 Course::sShakeMaxAngleMini = 15.0f;
s16 Course::sShakeFirstRippleTimeMini = 1000;
s16 Course::sShakeRippleTimeMini = 900;
s16 Course::sShakeRoughTimeMini = 300;
s16 Course::sShakeKeepTimeMini = 500;
s16 Course::sShakeRecoverTimeMini = 300;

#include "mathHelper.h"

const u8 Course::cGXTexMtxLabel[2] = {
    GX_TEXMTX0, GX_TEXMTX1};

const ResMgr::CourseDataId Course::cCourseTexSRTAnmTable[] = {
    ResMgr::mcCourseBtk1,
    ResMgr::mcCourseBtk2,
    ResMgr::mcCourseBtk3};

const ResMgr::CourseDataId Course::cCourseTevRegAnmTable[] = {
    ResMgr::mcCourseBrk};

using namespace JGeometry;
using namespace JUtility;

// TODO: Inline function ordering is incorrect here(which only affects debug), rn the default ctor for ExModel gets put in after isSkyFollow and RaceMgr::getManager
Course::Course(CrsData *data, int cameraCount, bool mirror)
{
    ECourseID courseID = data->getCourseID();
    mCrsID = courseID;
    _4 = 1;

    if (RaceMgr::getManager()->isRaceModeMiniGame())
        _4 = 0;

    mFogType = data->getFogType();
    mFogStartZ = data->getFogStartZ();
    mFogEndZ = data->getFogEndZ();

#ifdef DEBUG
    if (sForceDebugFogSetting)
    {
        mFogStartZ = 0.0f;
        mFogEndZ = 3000.0f;
    }
#endif

    mFogNearZ = 10.0f;
    mFogFarZ = 200000.0f;
    data->getFogColor(&mFogColor);
    mCameraCount = cameraCount;

    for (int i = 0; i < 3; i++)
        mTexSRTKey[i] = nullptr;

    mTexPattern = nullptr;
    mTevRegKey[0] = nullptr;
    _15c = nullptr;
    _180 = nullptr;
    mMatFlags = nullptr;
    mNormMatNum = 0;
    mHighMatNum = 0;
    mNormMatIDTable = nullptr;
    mHighMatIDTable = nullptr;
    mCrsData = data;
    mJugemPointNum = 0;
    mJugemPoint = nullptr;
    mCenterPoint = nullptr;

    for (int i = 0; i < 8; i++)
        mCLPoints[i] = nullptr;

    mTotalSectors = 0;
    mTrackSectorNum = 0;
    mSectors = nullptr;
    mTrackSectorDist = 0.0f;
    mPathNum = 0;
    mAreaNum = 0;
    mAreas = nullptr;
    mSkyMode = data->isSkyFollow();
    mIsTextCourse25 = false;
    if (mCrsID == TEST_25)
        mIsTextCourse25 = true;

    loadCrsData(mirror);
    reset();
    loadModelData();
}

void Course::reset()
{
    mZNorm.set(0.0f, 0.0f, 1.0f);
    mYNorm.set(0.0f, 1.0f, 0.0f);
    mXNorm.set(1.0f, 0.0f, 0.0f);
    mShakeZNorm.set(mZNorm);
    mShakeYNorm.set(mYNorm);
    mShakeXNorm.set(mXNorm);
    mPos.zero();

    switch (mCrsID)
    {
    case MINI_5: // TODO: Minigame ID -> Name
    case TEST_18:
        mPos.set(0.0f, 10000.0f, 0.0f);
        break;
    }

    mDirZ.set(mZNorm);
    mDirY.set(mYNorm);
    mDirX.set(mXNorm);

    if (!RaceMgr::getManager()->isRaceModeMiniGame())
    {
        mShakeMaxAngle = sShakeMaxAngle;
        mShakeFirstRippleTime = sShakeFirstRippleTime;
        mShakeRippleTime = sShakeRippleTime;
        mShakeRoughTime = sShakeRoughTime;
        mShakeKeepTime = sShakeKeepTime;
        mShakeRecoverTime = sShakeRecoverTime;

        if (RaceMgr::getManager()->isCrsDemoMode())
        {
            mShakeFirstRippleTime = sShakeFirstRippleTimeDemo;
        }
    }
    else
    {
        mShakeMaxAngle = sShakeMaxAngleMini;
        mShakeFirstRippleTime = sShakeFirstRippleTimeMini;
        mShakeRippleTime = sShakeRippleTimeMini;
        mShakeRoughTime = sShakeRoughTimeMini;
        mShakeKeepTime = sShakeKeepTimeMini;
        mShakeRecoverTime = sShakeRecoverTimeMini;
    }
    mShakeEnabled = true;

    if (getCrsData()->getShaking() == 2)
        _27e = 0;
    else
        _27e = 1;

    mShakeState = 0;
    mShakeTime = mShakeRippleTime - mShakeFirstRippleTime;
    mShakeAngleFlag = 0;

    for (int i = 0; i < 3; i++)
    {
        if (isTexSRTAnmEnable(i))
            mTexSRTAnm[i].resetFrame();
    }

    if (isTexPatAnmEnable())
        mTexPatAnm.resetFrame();

    for(int i = 0; i < 1; i++) {
        if (isTevRegAnmEnable(i))
            mTevRegAnm[i].resetFrame();
    }

    if (_15c)
        mSkyBtkMat.resetFrame();

    if (_180)
        _184.resetFrame();

    _530 = 0xff;
    _534 = 0;
    _538 = 0.0f;
}

Course::~Course() {}

void Course::loadCrsData(bool mirror)
{
#line 1287
    JUT_ASSERT_MSG(mCrsID != 0, "CRSID UNDEFINED");

    if (mCrsData->getCheckPathNumber() > 0)
    {
        createSector();
    }

    createJugemPoint();
    createCLPoint(mirror);
    mPathNum = mCrsData->getPathNumber();

    mAIPaths = new AIPath[mPathNum];

    for (int path = 0; path < mPathNum; path++)
    {
        CrsData::PathData *pathData = mCrsData->getPathData(path);
        AIPath *aiPath = &mAIPaths[path];

        int pointCount = pathData->pointCount;
        aiPath->init(pointCount);
        for (int point = 0; point < pointCount; point++)
        {
            AIPoint *aiPoint = aiPath->getPoint(point);
            CrsData::PointData *pointData = mCrsData->getPointData(path, point);
            aiPoint->setPosition((Vec *)&pointData->pos);
        }
    }
    createArea();
}

void Course::createSector()
{
    int checkPathCount = mCrsData->getCheckPathNumber();
    CrsData::CheckPath *checkPathTable = mCrsData->getCheckPathTable();
    CrsData::SCheckPoint *checkPointTable = mCrsData->getCheckPointTable();

    mTotalSectors = 0;
    mTrackSectorNum = 0;

    for (int i = 0; i < checkPathCount; i++)
    {
        CrsData::CheckPath *path = &checkPathTable[i];
        mTotalSectors += (s16)(path->getELinkNum() + (path->pointCount - 1));
        if (path->isMainPath())
            mTrackSectorNum += (s16)path->pointCount;
    }

    mSectors = new Sector[mTotalSectors];
    mMainSectors = new Sector *[mTrackSectorNum];

    for (int i = 0; i < mTrackSectorNum; i++)
        mMainSectors[i] = nullptr;

    int sectorIdx = 0;
    for (int i = 0; i < checkPathCount; i++)
    {
        CrsData::CheckPath *path = &checkPathTable[i];
        int offset = mCrsData->getCheckPointOffset(i);
        int j = 0;
        for (int j = 0; j < path->pointCount - 1; j++)
        {
            s16 id = offset + j;

            CrsData::SCheckPoint *cp1 = &checkPointTable[id];
            Sector *sector = &mSectors[sectorIdx];
            sector->setID(id);
            sector->setPointData(cp1, cp1 + 1, path->isMainPath());
            sectorIdx++;
        }

        for (int divNo = 0; divNo < path->getELinkNum(); divNo++)
        {
#line 1388
            JUT_MINMAX_ASSERT(0, divNo, 4);
            int nextOffset = path->next[divNo];

            CrsData::CheckPath *checkPath = &checkPathTable[nextOffset];
            int cpOffset = mCrsData->getCheckPointOffset(nextOffset);

            CrsData::SCheckPoint *cp1 = &checkPointTable[offset + path->pointCount - 1];
            CrsData::SCheckPoint *cp2 = &checkPointTable[cpOffset];
            bool isDivMain = (path->isMainPath() && checkPath->isMainPath());

            Sector *sector = mSectors + sectorIdx;
            sector->setPointData(cp1, cp2, isDivMain);

            if (path->getELinkNum() > 1)
            {
                sector->setDivFlag();
            }
            sectorIdx++;
        }
    }

    for (int i = 0; i < mTotalSectors; i++)
    {
        Sector *prev = &mSectors[i];
#line 1415
        JUT_ASSERT_MSG(prev->mFPoint, "SECTOR FPOINT UNLINK")
        JUT_ASSERT_MSG(prev->mBPoint, "SECTOR BPOINT UNLINK")

        for (int j = 0; j < mTotalSectors; j++)
        {

            if (i != j)
            {
                Sector *next = &mSectors[j];
                if (prev->mFPoint == next->mBPoint)
                {
                    prev->setNextSector(next);
                    next->setPrevSector(prev);
                }
            }
        }
    }

    for (int i = 0; i < mTotalSectors; i++)
        mSectors[i].calcGeneration(mSectors);

    for (int i = 0; i < mTotalSectors; i++)
    {
        Sector *prev = &mSectors[i];
        for (int j = 0; j < mTotalSectors; j++)
        {
            Sector *next = &mSectors[j];

            if (next->isMainSector())
            {
                if (prev->getGeneration() == next->getGeneration())
                {
                    prev->mMainSector = next;
                    break;
                }
            }
        }
    }

    Sector *prev = nullptr;
    Sector *next = mSectors;
    do
    {
        int tableIndex = next->getGeneration();
#line 1463
        JUT_MINMAX_ASSERT(0, tableIndex, mTrackSectorNum)
        mMainSectors[tableIndex] = next;
        mTrackSectorDist += next->getSectorDist();

        if (prev)
        {
            next->mTotalPriorDist = prev->mTotalPriorDist + prev->getSectorDist();
        }
        else
        {
            next->mTotalPriorDist = 0.0f;
        }

        for (int i = 0; i < next->getNextNum(); i++)
        {
            if (next->getNextSector(i)->isMainSector())
            {
                prev = next;
                next = next->getNextSector(i);
                break;
            }
        }

    } while (next != mSectors);
}

void Course::createJugemPoint()
{
    CrsData::SJugemPoint *respawnPoints = mCrsData->getJugemPointTable();
    int respawnpointCount = mCrsData->getJugemPointNumber();

    if (respawnpointCount > 0)
    {
        mJugemPointNum = respawnpointCount;
        mJugemPoint = new JugemPoint[respawnpointCount];

        for (int i = 0; i < respawnpointCount; i++)
        {
            mJugemPoint[i].setPointData(&respawnPoints[i]);
        }
    }
}

void Course::setLinkJugemPoint(CLPoint *linkPoint, u16 id)
{
    for (int i = 0; i < mJugemPointNum; i++)
    {
        JugemPoint *jugemPoint = &mJugemPoint[i];
        if (jugemPoint->isSameCPointID(id))
            jugemPoint->setCLPoint(linkPoint);
    }
}

JugemPoint *Course::searchJugemPoint(u16 id)
{
    JugemPoint *foundPoint = nullptr;
    for (int i = 0; i < mJugemPointNum; i++)
    {
        JugemPoint *jugemPoint = &mJugemPoint[i];
        if (jugemPoint->isSameJPointID(id))
        {
            foundPoint = jugemPoint;
            break;
        }
    }
#line 1574
    JUT_ASSERT_F(foundPoint, "JUGEM POINT NOT FOUND:ID(%d)", id)
    return foundPoint;
    ;
}

Course::Sector *Course::searchSector(s16 id)
{
    Sector *foundSector = nullptr;

    if (id >= 0)
    {
        for (int i = 0; i < mTotalSectors; i++)
        {
            Sector *sector = &mSectors[i];
            if (sector->getID() == id)
            {
                foundSector = sector;
                break;
            }
        }
    }

    return foundSector;
}

void Course::createCLPoint(bool mirror)
{
    CrsData::SCLPoint *clPointTable = mCrsData->getCLPointTable();
    int clPointCnt = mCrsData->getCLPointNumber();

    if (clPointCnt <= 0)
        return;

    CLPoint *clPoints = new CLPoint[clPointCnt];
    bool setDiv = true;
    CLPoint *prevLinkPoint = nullptr;
    for (int i = 0; i < clPointCnt; i++)
    {
        CLPoint *linkPoint = &clPoints[i];
        CrsData::SCLPoint *pointData = &clPointTable[i];
        linkPoint->setPointData(pointData);
        if (pointData->groupID >= 0)
        {
            setDiv = setDiv == false;
            if (setDiv)
            {
                linkPoint->setKind(2);
                linkPoint->mDivPoint = prevLinkPoint;
                prevLinkPoint->mDivPoint = linkPoint;
            }
            else
            {
                linkPoint->setKind(1);
                prevLinkPoint = linkPoint;
            }
        }
        else
        {
            linkPoint->setKind(0);
        }
        setLinkJugemPoint(linkPoint, i);
    }

    CLPoint *prevPoint = nullptr;
    for (int i = 0; i < clPointCnt; i++)
    {
        CLPoint *clPoint = &clPoints[i];
        const CrsData::SCLPoint *pointData = clPoint->getPointData();
        switch (clPoint->getKind())
        {
        case 1:
        {
            if (pointData->groupID != 0)
            {
                for (int j = 0; j < clPointCnt; j++)
                {
                    CLPoint *prevPoint = &clPoints[j];
                    const CrsData::SCLPoint *prevPointData = prevPoint->getPointData();
                    if (i != j && prevPoint->getKind() == 2 &&
                        prevPointData->groupID == pointData->groupID)
                    {
                        prevPoint->setNextPoint(clPoint);
                        clPoint->setPrevPoint(prevPoint);
                    }
                }
            }
            else
            {
                mCenterPoint = clPoint;
            }
            prevPoint = clPoint;

            break;
        }
        case 2:
        {
            prevPoint->setNextPoint(clPoint);
            clPoint->setPrevPoint(prevPoint);
            prevPoint = clPoint;
            if (pointData->groupID == 0)
            {
                clPoint->setNextPoint(mCenterPoint);
                mCenterPoint->setPrevPoint(clPoint);
            }
            break;
        }
        default:
        {
            prevPoint->setNextPoint(clPoint);
            clPoint->setPrevPoint(prevPoint);
            prevPoint = clPoint;
            break;
        }
        }
    }

    if (mirror)
    {
        mCLPoints[0] = mCenterPoint;
        for (u16 i = 1; i < 8; i++)
        {
            CrsData::PathData *pathData = mCrsData->getPathData(i);
            int pointCnt = pathData->getPointNumber();
            CLPoint *linkPoints = new CLPoint[pointCnt];
            mCLPoints[i] = linkPoints;
            CLPoint *prevLink = nullptr;
            CLPoint *nextLink = nullptr;

            for (u16 pointIdx = 0; pointIdx < pointCnt; pointIdx++)
            {
                CrsData::PointData *pointData = mCrsData->getPointData(i, pointIdx);
                CLPoint *linkPoint = &linkPoints[pointIdx];

                CrsData::SCLPoint *sclPoint = new CrsData::SCLPoint();
                memset(sclPoint, 0, sizeof(CrsData::SCLPoint));
                sclPoint->pos.set(pointData->pos);
                sclPoint->scale = 1.0f;
                linkPoint->setPointData(sclPoint);

                if (pointIdx == 0)
                {
                    linkPoint->setKind(1);
                    nextLink = linkPoint;
                }
                else if (pointIdx == (pointCnt - 1))
                {
                    linkPoint->setKind(2);
                    linkPoint->mDivPoint = nextLink;
                    nextLink->mDivPoint = linkPoint;

                    prevLink->setNextPoint(linkPoint);
                    linkPoint->setPrevPoint(prevLink);
                    linkPoint->setNextPoint(nextLink);
                    nextLink->setPrevPoint(linkPoint);
                }
                else
                {
                    linkPoint->setKind(0);
                    prevLink->setNextPoint(linkPoint);
                    linkPoint->setPrevPoint(prevLink);
                }
                prevLink = linkPoint;
            }
        }

        for (int jpointIdx = 0; jpointIdx < mJugemPointNum; jpointIdx++)
        {
            JugemPoint *jugemPoint = &mJugemPoint[jpointIdx];
            TVec3f frDir;
            jugemPoint->getFrDirection(&frDir);
            TVec3f pos;
            jugemPoint->getPosition(&pos);

            u16 jID = jugemPoint->getID();
            u16 pathID = jID / 100;
#line 1779
            JUT_MAX_ASSERT(pathID, 8)

            f32 prevLen = 0.0f;

            CLPoint *pathPoint = mCLPoints[pathID];
            CLPoint *clPoint = nullptr;
            do
            {
                TVec3f pathPos;
                pathPoint->getPosition(&pathPos);

                TVec3f displacement;
                displacement.sub(pathPos, pos);
                f32 len = displacement.length();
                displacement.normalize();
                f32 dp = displacement.dot(frDir);

                if (dp > 0.0f && (clPoint == nullptr || len < prevLen))
                {
                    clPoint = pathPoint;
                    prevLen = len;
                }

                pathPoint = pathPoint->getNextPoint(0);
            } while (pathPoint != mCLPoints[pathID]);

#line 1805
            JUT_ASSERT_F(clPoint, "MIN NOT FOUND J:%d", jID)

            if (clPoint)
            {
                jugemPoint->setCLPoint(clPoint);
            }
        }
    }

#ifdef DEBUG
    for (int i = 0; i < clPointCnt; i++)
    {
        CLPoint *clPoint = &clPoints[i];
        int prevPointCnt = clPoint->getPrevPointNumber();
#line 1824
        JUT_ASSERT_F(prevPointCnt > 0, "CLPOINT[%d]:PREV = 0", i);

        for (int j = 0; j < prevPointCnt; j++)
        {
            CLPoint *prev = clPoint->getPrevPoint(j);
            JUT_ASSERT_F(prev, "CLPOINT[%d]:PREV[%d]", i, j);
        }

        int nextPointCnt = clPoint->getNextPointNumber();
        JUT_ASSERT_F(nextPointCnt > 0, "CLPOINT[%d]:NEXT = 0", i);

        for (int j = 0; j < nextPointCnt; j++)
        {
            CLPoint *next = clPoint->getNextPoint(j);
            JUT_ASSERT_F(next, "CLPOINT[%d]:NEXT[%d]", i, j);
        }
    }
#endif
}

CLPoint *Course::getFirstCLPoint(int kartNo) const
{
    if (mCLPoints[0])
    {
#line 1852
        JUT_MINMAX_ASSERT(0, kartNo, 8)
        return mCLPoints[kartNo];
    }

    return mCenterPoint;
}

void Course::createArea()
{
    CrsData::SArea *areaTable = mCrsData->getAreaTable();
    mAreaNum = mCrsData->getAreaNumber();
    mAreas = new Area[mAreaNum];

    for (int i = 0; i < mAreaNum; i++)
        mAreas[i].init(&areaTable[i]);
}

int Course::getShadowAreaNumber()
{
    return mAreas[0].getKind();
}

static void smpCalcMaterial(J3DModelData *modelData, J3DJoint *joint, Mtx m)
{
    Mtx r_mtx;
    Mtx next_mtx;
    J3DGetTranslateRotateMtx(joint->getTransformInfo(), r_mtx);
    PSMTXConcat(m, r_mtx, next_mtx);
    modelData->simpleCalcMaterial(joint->getJntNo(), next_mtx);
    J3DJoint *child = joint->getChild();
    if (child)
        smpCalcMaterial(modelData, child, next_mtx);

    J3DJoint *younger = joint->getYounger();
    if (younger)
        smpCalcMaterial(modelData, younger, next_mtx);
}

void Course::setFogInfo(J3DModelData *modelData)
{
    ExModel::setFogInfo(modelData, mFogType, mFogStartZ, mFogEndZ, mFogNearZ, mFogFarZ, &mFogColor);
}

void Course::setFogInfo(ExModel *model)
{
    ExModel::setFogInfo(model->getModelData(), mFogType, mFogStartZ, mFogEndZ, mFogNearZ, mFogFarZ, &mFogColor);
}

void Course::loadModelData()
{
    SYSDBG_SetHeapGroup("COURSE MODEL", nullptr);
    void *courseModel = ResMgr::getPtr(ResMgr::mcCourseBmd);
    if (courseModel)
    {
        J3DModelData *modelData = J3DModelLoaderDataBase::load(courseModel, 0x19220010);
        ExModel::patchModelData(modelData);
        mTexModel.setModelData(modelData);
        ExModel::setFogInfo(modelData, mFogType, mFogStartZ, mFogEndZ, mFogNearZ, mFogFarZ, &mFogColor);
        mTexModel.setLODBias(TexLODControl::getGeographyLODBias());

        for (int i = 0; i < 3; i++)
        {
            void *texSRTAnm = ResMgr::getPtr(cCourseTexSRTAnmTable[i]);
            if (texSRTAnm)
            {
                J3DAnmObjMaterial::setupTexSRTAnmData(&mTexSRTKey[i], modelData, texSRTAnm);
                mTexSRTAnm[i].setExModel(&mTexModel);
                mTexSRTAnm[i].attach(mTexSRTKey[i]);
            }
        }

        void *texPat = ResMgr::getPtr(ResMgr::mcCourseBtp);
        if (texPat)
        {
            J3DAnmObjMaterial::setupTexPatternAnmData(&mTexPattern, modelData, texPat);
            mTexPatAnm.setExModel(&mTexModel);
            mTexPatAnm.attach(mTexPattern);
        }

        for (int i = 0; i < 1; i++)
        {
            void *tevReg = ResMgr::getPtr(cCourseTevRegAnmTable[i]);
            if (tevReg)
            {
                J3DAnmObjMaterial::setupTevRegAnmData(mTevRegKey, modelData, tevReg);
                mTevRegAnm[i].setExModel(&mTexModel);
                mTevRegAnm[i].attach(mTevRegKey[i]);
            }
        }

        modelData->newSharedDisplayList(0x40000);
        smpCalcMaterial(modelData, modelData->getJointNodePointer(0), j3dDefaultMtx);
        modelData->makeSharedDL();

        mMatFlags = new u16[modelData->getMaterialNum() & 0xffff];
        for (u16 i = 0; i < modelData->getMaterialNum(); i++)
            mMatFlags[i] = 0;

        mNormMatIDTable = new u32[modelData->getMaterialInitNum() & 0xffff];
        mHighMatIDTable = new u32[modelData->getMaterialInitNum() & 0xffff];

        for (int i = 0; i < modelData->getMaterialInitNum(); i++)
        {
            mNormMatIDTable[i] = 0xffffffff;
            mHighMatIDTable[i] = 0xffffffff;
        }

        for (u16 i = 0; i < modelData->getMaterialNum(); i++)
        {
            J3DMaterial *node = modelData->getMaterialNodePointer(i);

            if (node->isDrawModeOpaTexEdge())
            {

                bool found = searchMaterial(node->getMaterialID(), mNormMatIDTable, mNormMatNum) == false;
                if (found)
                {
#line 2091
                    JUT_MINMAX_ASSERT(0, mNormMatNum, modelData->getMaterialInitNum())
                    JUT_ASSERT(mNormMatIDTable[mNormMatNum] == 0xffffffff)
                    mNormMatIDTable[mNormMatNum] = node->getMaterialID();
                    mNormMatNum++;
                }
            }
        }

        for (u16 i = 0; i < modelData->getMaterialNum(); i++)
        {
            J3DMaterial *node = modelData->getMaterialNodePointer(i);

            if (node->isDrawModeXlu())
            {
                const char *name = modelData->getMaterialName()->getName(i);
                if (!isHighPriority(name))
                {
                    bool found = searchMaterial(node->getMaterialID(), mNormMatIDTable, mNormMatNum) == false;
                    if (found)
                    {
#line 2108
                        JUT_MINMAX_ASSERT(0, mNormMatNum, modelData->getMaterialInitNum())
                        JUT_ASSERT(mNormMatIDTable[mNormMatNum] == 0xffffffff)
                        mNormMatIDTable[mNormMatNum] = node->getMaterialID();
                        mNormMatNum++;
                    }
                }
            }
        }

        for (u16 i = 0; i < modelData->getMaterialNum(); i++)
        {
            J3DMaterial *node = modelData->getMaterialNodePointer(i);

            if (node->isDrawModeXlu())
            {
                const char *name = modelData->getMaterialName()->getName(i);
                if (isHighPriority(name))
                {
                    bool found = searchMaterial(node->getMaterialID(), mHighMatIDTable, mHighMatNum) == false;
                    if (found)
                    {
#line 2127
                        JUT_MINMAX_ASSERT(0, mHighMatNum, modelData->getMaterialInitNum())
                        JUT_ASSERT(mHighMatIDTable[mHighMatNum] == 0xffffffff)
                        mHighMatIDTable[mHighMatNum] = node->getMaterialID();
                        mHighMatNum++;
                    }
                }
            }
        }
    }

    SYSDBG_SetHeapGroup("COURSE MODEL", nullptr);
    void *skyModel = ResMgr::getPtr(ResMgr::mcCourseSkyBmd);

    if (skyModel)
    {
        J3DModelData *modelData = J3DModelLoaderDataBase::load(skyModel, 0x19220010);
        ExModel::patchModelData(modelData);
        for (int skyNo = 0; skyNo < mCameraCount; skyNo++)
        {
#line 2160
            JUT_MINMAX_ASSERT(0, skyNo, 4)
            mSkyModel[skyNo].setModelData(modelData);
        }

        void *texKey = ResMgr::getPtr(ResMgr::mcCourseSkyBtk);
        if (texKey)
        {
            J3DAnmObjMaterial::setupTexSRTAnmData(&_15c, modelData, texKey);
            mSkyBtkMat.setExModel(mSkyModel);
            mSkyBtkMat.attach(_15c);
        }

        void *regKey = ResMgr::getPtr(ResMgr::mcCourseSkyBrk);
        if (regKey)
        {
            J3DAnmObjMaterial::setupTevRegAnmData(&_180, modelData, regKey);
            _184.setExModel(mSkyModel);
            _184.attach(_180);

            for (int skyNo = 0; skyNo < mCameraCount; skyNo++)
            {
#line 2187
                JUT_MINMAX_ASSERT(0, skyNo, 4)
                mSkyModel[skyNo].setSimpleTevReg(2);
            }
        }

        modelData->newSharedDisplayList(0x40000);
        modelData->simpleCalcMaterial(j3dDefaultMtx);
        modelData->makeSharedDL();
    }

    SYSDBG_SetHeapGroup("COURSE MGR", nullptr);
}

bool Course::searchMaterial(u32 id, u32 *table, s16 cnt)
{
    bool ret = false;
    for (int i = 0; i < cnt; i++)
    {
        if (id == table[i])
        {
            ret = true;
            break;
        }
    }
    return ret;
}

bool Course::isHighPriority(const char *name)
{
    u32 n = 0;
    bool highPrio = false;

    if (name)
        n = strlen(name);

    bool readNext = false;
    int offset = 1;
    do
    {
        if (n >= 4)
        {
            u32 nameU32 = 0;
            for (int i = 0; i < 4; i++)
            {
                nameU32 |= (u8)name[(n - offset) - i] << (i * 8);
            }

            readNext = false;
            switch (nameU32)
            {
            case '_HIG':
                highPrio = true;
                continue;
            case '_v_x':
                readNext = true;
                offset += 4;
                continue;
            }

            if ((u16)nameU32 == '_v')
            {
                readNext = true;
                offset += 2;
            }
        }

    } while (readNext);

    return highPrio;
}

void Course::createModel(JKRSolidHeap *heap, u32) {}

void Course::calcShaking()
{
    if (!isShaking())
        return;
    if (!mShakeEnabled)
        return;

    f32 angle = 0.0f;
    mShakeTime++;

    switch (mShakeState)
    {
    case 1:
    {
        if (!RaceMgr::getManager()->isRaceModeMiniGame())
        {
            int roughTime = mShakeRoughTime;
            f32 rem = mShakeTime - roughTime;
            f32 rad = roughTime * roughTime;
            angle = 1.0f + (-1.0f / rad) * rem * rem;
        }
        else
        {
            angle = 0.5f * JMASSin(32768.0f * ((f32)mShakeTime / (f32)mShakeRoughTime) + -16384.0f) + 0.5f;
        }
        if (mShakeTime >= mShakeRoughTime)
        {
            mShakeState = 2;
            mShakeTime = 0;
        }
        break;
    }

    case 2:
    {
        angle = 1.0f;
        if (mShakeTime >= mShakeKeepTime)
        {
            mShakeState = 3;
            mShakeTime = 0;
        }
        break;
    }

    case 3:
    {
        if (!RaceMgr::getManager()->isRaceModeMiniGame())
        {
            angle = 1.0f - ((f32)mShakeTime / (f32)mShakeRecoverTime);
        }
        else
        {
            f32 rem = (mShakeTime - mShakeRecoverTime);
            f32 rad = (f32)(mShakeRecoverTime * mShakeRecoverTime);
            angle = 1.0f - (1.0f + (-1.0f / rad) * rem * rem);
        }

        if (mShakeTime >= mShakeRecoverTime)
        {
            mShakeState = 0;
            mShakeTime = 0;
            mShakeAngleFlag ^= 1;
        }

        break;
    }

    default:
    {
        if (mShakeTime == (mShakeRippleTime - 180))
            if (RaceMgr::getManager()->isRaceModeMiniGame())
                GetGameAudioMain()->startRaceSystemSe(0x20024);

        if (mShakeTime >= mShakeRippleTime)
        {
            mShakeState = 1;
            mShakeTime = 0;
            if (RaceMgr::getManager()->isRaceModeMiniGame())
                GetGameAudioMain()->startRaceSystemSe(0x20022);
        }
    }
    break;
    }

    mShakeYNorm.set(mYNorm);
    mShakeZNorm.set(mZNorm);
    mShakeXNorm.set(mXNorm);
    f32 angleRad = TAU * ((mShakeMaxAngle * angle) / 360.0f);

    if (mShakeAngleFlag != 0)
    {
        angleRad = -angleRad;
    }

    f32 rad = std::tan(angleRad);
    TVec3f xNorm(1.0f, 0.0f, 0.0f);
    TVec3f zNorm(0.0f, 0.0f, 1.0f);

    if (_27e != 0)
    {
        rad = -rad;
        mDirY.set(0.0f, 1.0f, 0.0f);
        if (sShakeDir == 0)
            mDirY.scaleAdd(rad, zNorm, mDirY);
        else
            mDirY.scaleAdd(rad, xNorm, mDirY);

        mDirY.normalize();
        mDirZ.cross(mDirX, mDirY);
        mDirZ.normalize();
        mDirX.cross(mDirY, mDirZ);
    }
    else
    {
        mYNorm.set(0.0f, 1.0f, 0.0f);
        if (sShakeDir == 0)
            mYNorm.scaleAdd(rad, zNorm, mYNorm);
        else
            mYNorm.scaleAdd(rad, xNorm, mYNorm);

        mYNorm.normalize();
        mZNorm.cross(mXNorm, mYNorm);
        mZNorm.normalize();
        mXNorm.cross(mYNorm, mZNorm);
    }
}

void Course::calc()
{
#ifdef DEBUG
    if (sConfigFog)
    {
        ExModel::setFogInfo(mTexModel.getModelData(), mFogType, mFogStartZ, mFogEndZ, mFogNearZ, mFogFarZ, &mFogColor);
        mTexModel.getModelData()->makeSharedDL();
    }
#endif

    for (int index = 0; index < mCameraCount; index++)
    {
        TVec3f *camPos = RaceMgr::getManager()->getCamera(index)->GetCameraPos();
#line 2484
        JUT_MINMAX_ASSERT(0, index, 4)

        mCamMtx.setXYZDir(mDirX, mDirY, mDirZ);

        if (mSkyMode != 0)
        {
            mCamMtx[0][3] = camPos->x;
            mCamMtx[1][3] = camPos->y;
            mCamMtx[2][3] = camPos->z;
        }
        else
        {
            mCamMtx[0][3] = 0.0f;
            mCamMtx[1][3] = 0.0f;
            mCamMtx[2][3] = 0.0f;
        }

        mSkyModel[index].setBaseTRMtx(mCamMtx);
    }

    for (int i = 0; i < 3; i++)
    {
        if (isTexSRTAnmEnable(i))
            mTexSRTAnm[i].frameProc();
    }

    if (isTexPatAnmEnable())
        mTexPatAnm.frameProc();

    if (isTevRegAnmEnable(0))
        mTevRegAnm[0].frameProc();

    if (_15c)
        mSkyBtkMat.frameProc();

    if (_180)
        _184.frameProc();

    mTexMtx.setXYZDir(mXNorm, mYNorm, mZNorm);
    mTexMtx[0][3] = mPos.x;
    mTexMtx[1][3] = mPos.y;
    mTexMtx[2][3] = mPos.z;
    mTexModel.setBaseTRMtx(mTexMtx);

    J3DModelData *modelData = mTexModel.getModelData();

    if (modelData)
    {
        for (u32 cns = 0; cns < RaceMgr::getManager()->getConsoleNumber(); cns++)
        {
            KartCam *cam = RaceMgr::getManager()->getCamera(cns);
            for (u16 index = 0; index < modelData->getMaterialNum(); index++)
            {
                J3DMaterial *mat = modelData->getMaterialNodePointer(index);
                if (_4 != 0)
                {
                    J3DShape *shape = mat->getShape();
                    TVec3f sum;
                    TVec3f min(shape->getMin());
                    TVec3f max(shape->getMax());
                    sum.add(min, max);
                    sum.scale(0.5f);

                    TVec3f diff;
                    diff.sub(max, min);
                    f32 bounds = diff.length() * 0.5f;

                    Vec pos = *(Vec *)&sum;

                    if (cam->GetClipper()->clip(cam->GetMtx(), pos, bounds * sBoundsScale) == 0)
                        mMatFlags[index] &= ~(1 << cns);
                    else
                        mMatFlags[index] |= (1 << cns);
                }
                else
                {
                    mMatFlags[index] &= ~(1 << cns);
                }
            }
        }
    }
}

void Course::convWorldPosToMapCoord(TVec3f *mapcoord, const TVec3f &worldPos)
{
    if (isShaking())
    {
        TVec3f diff;
        diff.sub(worldPos, mPos);
        mapcoord->x = diff.dot(mXNorm);
        mapcoord->y = diff.dot(mYNorm);
        mapcoord->z = diff.dot(mZNorm);
        return;
    }
    mapcoord->set(worldPos);
}

void Course::convWorldPosToGridCoord(TVec3f *gridCoord, const TVec3f &worldPos)
{
    if (isShaking())
    {
        stPlaneParam plane;
        stMakePlaneParam(plane, mYNorm, mPos);
        TVec3f gridPos(worldPos);
        gridPos.y = -(plane.d + (plane.x * worldPos.x + plane.z * worldPos.z)) / plane.y;

        TVec3f diff;
        diff.sub(gridPos, mPos);
        gridCoord->x = diff.dot(mXNorm);
        gridCoord->y = 0.0f;
        gridCoord->z = diff.dot(mZNorm);
        return;
    }
    gridCoord->set(worldPos);
}

void Course::convMapCoordToWorldPos(TVec3f *worldPos, const TVec3f &mapcoord)
{
    if (isShaking())
    {
        worldPos->set(mPos);
        worldPos->scaleAdd(mapcoord.x, mXNorm, *worldPos);
        worldPos->scaleAdd(mapcoord.y, mYNorm, *worldPos);
        worldPos->scaleAdd(mapcoord.z, mZNorm, *worldPos);
        return;
    }
    worldPos->set(mapcoord);
}

void Course::calcGroundNormal(TVec3f *groundNormal, const TVec3f &dir)
{
    if (isShaking())
    {
        TVec3f normal(0.0f, 1.0f, 0.0f);
        TVec3f diff;
        diff.sub(dir, normal);
        groundNormal->add(mYNorm, diff);
        groundNormal->normalize();
        return;
    }
    groundNormal->set(dir);
}

void Course::calcPlaneParam(stPlaneParam *plane, f32 x, f32 y, f32 z, f32 d, const TVec3f &p)
{
    if (isShaking())
    {
        TVec3f dir(x, y, z);
        TVec3f groundNormal;
        calcGroundNormal(&groundNormal, dir);

        plane->x = groundNormal.x;
        plane->y = groundNormal.y;
        plane->z = groundNormal.z;
        plane->d = -(plane->x * p.x + plane->y * p.y + plane->z * p.z);
        return;
    }

    plane->x = x;
    plane->y = y;
    plane->z = z;
    plane->d = d;
}

void Course::getShakeVelocity(TVec3f *vel, const TVec3f &pos)
{
    if (isShaking() && sAddShakeVel)
    {
        TVec3f mapCoord;
        convWorldPosToMapCoord(&mapCoord, pos);

        TVec3f shakePos(mPos);
        shakePos.scaleAdd(mapCoord.x, mShakeXNorm, shakePos);
        shakePos.scaleAdd(mapCoord.y, mShakeYNorm, shakePos);
        shakePos.scaleAdd(mapCoord.z, mShakeZNorm, shakePos);
        vel->sub(pos, shakePos);
        return;
    }
    vel->zero();
}

void Course::update()
{
    mTexModel.update(0);
}

void Course::updateSky()
{
    for (int i = 0; i < mCameraCount; i++)
        mSkyModel[i].update(0);
}

void Course::drawBase(u32 cameraNo, u32 *matTable, s16 matNum, bool checkZMode)
{
    for (int i = 0; i < 3; i++)
    {
        if (isTexSRTAnmEnable(i))
            mTexSRTAnm[i].anmFrameProc();
    }

    if (isTexPatAnmEnable())
        mTexPatAnm.anmFrameProc();

    for (int i = 0; i < 1; i++)
    {
        if (isTevRegAnmEnable(i))
            mTevRegAnm[i].anmFrameProc();
    }

    if (mIsTextCourse25)
        return;

    J3DModelData *modeldata = mTexModel.getModelData();
    if (modeldata)
    {
        modeldata->syncJ3DSys();
        J3DJoint *node = modeldata->getJointNodePointer(0);
        const Mtx &baseTR = mTexModel.getBaseTRMtx();

        Mtx viewPos;
        Mtx t_m;
        Mtx m;

        J3DGetTranslateRotateMtx(node->getTransformInfo(), t_m);
        PSMTXConcat(baseTR, t_m, m);
        const Mtx &viewMtx = j3dSys.getViewMtx();

        PSMTXConcat(viewMtx, m, viewPos);
        GXLoadPosMtxImm(viewPos, 0);
        GXLoadNrmMtxImm(viewPos, 0);
        J3DShape::resetVcdVatCache();

        for (u16 i = 0; i < matNum; i++)
        {
            bool unk = false;
            for (J3DMaterial *mat = node->getMesh(); mat != nullptr; mat = mat->getNext())
            {
#line 2818
                JUT_MINMAX_ASSERT(0, mat->getIndex(), modeldata->getMaterialNum())
                bool doUpdate = false;
                if (matTable[i] == mat->getMaterialID() && ((1 << cameraNo & mMatFlags[mat->getIndex()]) == 0))
                {
                    doUpdate = true;
                    if (checkZMode && mat->getPEBlock()->getZMode()->getUpdateEnable() != 1)
                        doUpdate = false;
                }

                if (!doUpdate)
                    continue;

                if (!unk)
                {
                    mat->loadSharedDL();
                    mat->getShape()->loadPreDrawSetting();

                    if (mat->getMaterialAnm())
                    {
                        mat->getMaterialAnm()->calc(mat);
                    }
                    mat->calc(m);

                    for (u32 j = 0; j < 2; j++)
                    {
                        J3DTexMtx *texMtx = mat->getTexMtx(j);
                        if (texMtx)
                        {
                            J3DTexMtxInfo &texMtxInfo = texMtx->getTexMtxInfo();
                            bool loadTex = false;
                            switch (texMtxInfo.mInfo & 0x3f)
                            {
                            case 6:
                            case 7:
                            {
                                Mtx texView;
                                PSMTXCopy(viewPos, texView);
                                texView[0][3] = 0.0f;
                                texView[1][3] = 0.0f;
                                texView[2][3] = 0.0f;
                                texMtx->calc(texView);
                                loadTex = true;
                                break;
                            }
                            }
                            if (loadTex)
                            {
                                GXLoadTexMtxImm(texMtx->getMtx(), (j * 3) + 30, (GXTexMtxType)texMtx->getTexMtxInfo().mProjection);
                            }
                        }
                    }

                    if (mat->getMaterialAnm())
                    {
                        if (isTexPatAnmEnable())
                        {
                            u16 texNo = mat->getTevBlock()->getTexNo(0);
                            modeldata->getTexture()->loadGX(texNo, GX_TEXMAP0);
                        }

                        if (isTexSRTAnmEnable(0))
                        {
                            for (u32 j = 0; j < 2; j++)
                            {
                                J3DTexMtx *texMtx = mat->getTexGenBlock()->getTexMtx(j);
                                if (texMtx)
                                {
                                    GXLoadTexMtxImm(texMtx->getMtx(), cGXTexMtxLabel[j], (GXTexMtxType)texMtx->getTexMtxInfo().mProjection);
                                }
                            }
                        }

                        if (isTevRegAnmEnable(0))
                        {
                            GXSetTevColorS10(GX_TEVREG0, *mat->getTevBlock()->getTevColor(0));
                            GXSetTevKColor(GX_KCOLOR0, *mat->getTevBlock()->getTevKColor(0));
                        }
                    }
                    unk = true;
                }
                mat->getShape()->simpleDrawCache();
            }
        }
    }
}

void Course::drawSky(u32 viewNo)
{
    if (_15c)
        mSkyBtkMat.anmFrameProc();

    if (_180)
        _184.anmFrameProc();

#line 2930
    JUT_MAX_ASSERT(viewNo, 4)

    if (mSkyModel[viewNo].getModelData())
    {
        SimpleDrawer simpleDrawer;
        simpleDrawer.drawInit(&mSkyModel[viewNo]);

        while (simpleDrawer.loadPreDrawSetting())
        {
            mSkyModel[viewNo].simpleDraw(viewNo);
        }
    }
}

void Course::getStartLine(TVec3f *pos, TVec3f *dir)
{
    pos->set(0.0f, 0.0f, 0.0f);
    dir->set(0.0f, 0.0f, 1.0f);
    if (mCrsData)
    {
        CrsData::StartPoint *startPoint = mCrsData->getStartPoint(0xff);
        if (startPoint)
        {
            startPoint->getPosition(pos);
            startPoint->getFrDirection(dir);
        }
    }
}

void Course::getLightColor(TColor *dst)
{
    getCrsData()->getLightColor(dst);
}

void Course::getLightOffsetPosition(TVec3f *dst)
{
    getCrsData()->getLightOffsetPosition(dst);
}

void Course::getAmbientColor(TColor *dst)
{
    getCrsData()->getAmbientColor(dst);
}

u32 Course::getCourseBGM()
{
    u32 bgmID = 0x2000004;
    switch (mCrsID)
    {
    case BABY_PARK:
        bgmID = 0x2000000;
        break;
    case PEACH_BEACH:
        bgmID = 0x2000001;
        break;
    case DAISY_CRUISER:
        bgmID = 0x2000002;
        break;
    case LUIGI_CIRCUIT:
        bgmID = 0x2000003;
        break;
    case MARIO_CIRCUIT:
        bgmID = 0x2000004;
        break;
    case YOSHI_CIRCUIT:
        bgmID = 0x2000005;
        break;
    case COURSE_UNK2_27:
        break;
    case MUSHROOM_BRIDGE:
        bgmID = 0x2000006;
        break;
    case MUSHROOM_CITY:
        bgmID = 0x2000007;
        break;
    case WALUIGI_STADIUM:
        bgmID = 0x2000008;
        break;
    case WARIO_COLOSSEUM:
        bgmID = 0x2000009;
        break;
    case DINO_DINO_JUNGLE:
        bgmID = 0x200000a;
        break;
    case DK_MOUNTAIN:
        bgmID = 0x200000b;
        break;
    case COURSE_UNK3_2e:
        break;
    case BOWSER_CASTLE:
        bgmID = 0x200000c;
        break;
    case COURSE_UNK4_30:
        bgmID = 0x200000d;
        break;
    case RAINBOW_ROAD:
        bgmID = 0x200000d;
        break;
    case DRY_DRY_DESERT:
        bgmID = 0x200000e;
        break;
    case SHERBET_LAND:
        bgmID = 0x200000f;
        break;
    case MINI_1:
    case MINI_2:
    case MINI_3:
    case MINI_4:
    case MINI_5:
    case MINI_6:
    case MINI_7:
    case MINI_8:
    {
        ERaceMode raceMode = RaceMgr::getManager()->getRaceMode();
        if (raceMode == ESCAPE_BATTLE)
        {
            bgmID = 0x2000024;
        }
        else
        {
            switch (mCrsID)
            {
            case MINI_1:
                bgmID = 0x200000c;
                break;
            case MINI_5:
                bgmID = 0x200002e;
                break;
            default:
                bgmID = 0x2000023;
                break;
            }
        }
        break;
    }
    case REVERSE_1:
    case REVERSE_2:
    case REVERSE_3:
    case REVERSE_4:
    case REVERSE_5:
    case REVERSE_6:
    case REVERSE_7:
    case REVERSE_8:
        break;
    case AWARD:
    {
        if (RaceMgr::getManager()->getAwardKartNo() >= 0)
            bgmID = 0x2000027;
        else
            bgmID = 0x2000029;
        break;
    case ENDING:
        // The credits are sped up in 50hz, to accomodate for that, also use a sped up version of the BGM
        if (KartLocale::getVideoMode() == VideoMode_PAL && KartLocale::getVideoFrameMode() == VideoFrameMode_50Hz)
            bgmID = 0x200002b;
        else
            bgmID = 0x2000028;
        break;
    }
    }
    return bgmID;
}

u32 Course::getFinalLapBGM()
{
    u32 bgmID = 0x2000014; // default final lap ID, used for Mario Circuit and test tracks
    switch (mCrsID)
    {
    case BABY_PARK:
        bgmID = 0x2000010;
        break;
    case PEACH_BEACH:
        bgmID = 0x2000011;
        break;
    case DAISY_CRUISER:
        bgmID = 0x2000012;
        break;
    case LUIGI_CIRCUIT:
        bgmID = 0x2000013;
        break;
    case MARIO_CIRCUIT:
        bgmID = 0x2000014;
        break;
    case YOSHI_CIRCUIT:
        bgmID = 0x2000015;
        break;
    case COURSE_UNK2_27:
        break;
    case MUSHROOM_BRIDGE:
        bgmID = 0x2000016;
        break;
    case MUSHROOM_CITY:
        bgmID = 0x2000017;
        break;
    case WALUIGI_STADIUM:
        bgmID = 0x2000018;
        break;
    case WARIO_COLOSSEUM:
        bgmID = 0x2000019;
        break;
    case DINO_DINO_JUNGLE:
        bgmID = 0x200001a;
        break;
    case DK_MOUNTAIN:
        bgmID = 0x200001b;
        break;
    case COURSE_UNK3_2e:
        break;
    case BOWSER_CASTLE:
        bgmID = 0x200001c;
        break;
    case COURSE_UNK4_30:
        bgmID = 0x200001d;
        break;
    case RAINBOW_ROAD:
        bgmID = 0x200001d;
        break;
    case DRY_DRY_DESERT:
        bgmID = 0x200001e;
        break;
    case SHERBET_LAND:
        bgmID = 0x200001f;
        break;
    }
    return bgmID;
}

void Course::drawModelForDebug(u32 cameraNo)
{
    int areaCnt = mAreaNum;
    for (u8 i = 0; i < 8; i++)
    {
        if (sDrawAreaModel & 1 << i)
        {
            for (int areaNo = 0; areaNo < areaCnt; areaNo++)
            {
                Area *area = &mAreas[areaNo];
                if (i == area->getKind())
                    area->drawDebugModel(cameraNo);
            }
        }
    }
}

Course::Sector::Sector()
{
    mID = -1;
    mGeneration = -1;
    mIsMainSector = false;
    mDivFlag = false;
    mBPoint = nullptr;
    mFPoint = nullptr;
    mBDir.zero();
    mFDir.zero();
    mSectorDist = 0.0f;
    mPrevNum = 0;
    mNextNum = 0;

    for (int i = 0; i < 4; i++)
    {
        mPrevSectors[i] = nullptr;
        mNextSectors[i] = nullptr;
    }

    mMainSector = nullptr;

    for (int i = 0; i < 4; i++)
    {
        _10[i].zero();
        _40[i].zero();
    }

    mTotalPriorDist = 0.0f;
}

void Course::Sector::setPointData(const CrsData::SCheckPoint *bPoint, const CrsData::SCheckPoint *fPoint, bool isMain)
{
    mIsMainSector = isMain;
    TVec3f yNorm(0.0f, 1.0f, 0.0f);
    mBPoint = bPoint;
    mFPoint = fPoint;

    TVec3f bPointLen;
    TVec3f fPointLen;

    bPointLen.sub(mBPoint->cpStart, mBPoint->cpEnd);
    fPointLen.sub(mFPoint->cpEnd, mFPoint->cpStart);
    _10[0].set(mBPoint->cpEnd);
    _10[1].set(mBPoint->cpStart);
    _10[2].set(mFPoint->cpStart);
    _10[3].set(mFPoint->cpEnd);

    for (int i = 0; i < 4; i++)
    {
        _40[i].sub(_10[((i + 1) % 4)], _10[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        _10[i].y = 0.0f;
        _40[i].y = 0.0f;
    }

    TVec3f bPointDir;
    TVec3f fPointDir;

    bPointDir.cross(bPointLen, yNorm);
    mBDir.set(bPointDir.x, bPointDir.z);
    mBDir.normalize();

    fPointDir.cross(fPointLen, yNorm);
    mFDir.set(fPointDir.x, fPointDir.z);
    mFDir.normalize();

    bPointLen.y = 0.0f;
    fPointLen.y = 0.0f;

    bPointLen.scale(0.5f);
    fPointLen.scale(0.5f);

    TVec3f totalLen;
    totalLen.sub(bPointLen, fPointLen);
    mSectorDist = totalLen.length();
}

#pragma push
#pragma force_active on
const u32 unk_data[] = {
    0x00000064,
    0xFF000064,
    0xFFFF0064,
    0x00FF0064,
    0x00FFFF64,
    0xFFFFFF64,
    0xFF00FF64,
    0x8040FF64,
};
#pragma pop

void Course::Sector::setPrevSector(Sector *sector)
{
#line 3526
    JUT_ASSERT_MSG(mPrevNum < 4, "SECTOR PREV LINK OVERFLOW")

    JUT_MINMAX_ASSERT(0, mPrevNum, 4)
    mPrevSectors[mPrevNum] = sector;
    mPrevNum++;
}

void Course::Sector::setNextSector(Sector *sector)
{
#line 3536
    JUT_ASSERT_MSG(mNextNum < 4, "SECTOR NEXT LINK OVERFLOW")

    JUT_MINMAX_ASSERT(0, mNextNum, 4)
    mNextSectors[mNextNum] = sector;
    mNextNum++;
}

void Course::Sector::calcGeneration(Course::Sector *sector)
{
    u16 generation = 0;

    for (Sector *cur = this; cur != sector; cur = cur->getPrevSector(0))
        generation++;

    mGeneration = generation;
}

f32 Course::Sector::calcUnitDist(const TVec3f &pos)
{
    f32 unitDist = -1.0f;
    if (isLRInside(pos))
    {
        TVec2f bpointDiff;
        TVec2f fpointDiff;

        bpointDiff.x = pos.x - mBPoint->cpStart.x;
        bpointDiff.y = pos.z - mBPoint->cpStart.z;
        fpointDiff.x = pos.x - mFPoint->cpEnd.x;
        fpointDiff.y = pos.z - mFPoint->cpEnd.z;

        unitDist = bpointDiff.dot(mBDir);
        unitDist = unitDist / (unitDist + fpointDiff.dot(mFDir));

        if (unitDist < 0.0f && unitDist >= -0.001f)
            unitDist = 0.0f;
    }
    return unitDist;
}

bool Course::Sector::isLRInside(const TVec3f &pos)
{
    bool ret = true;
    TVec3f posXZ(pos.x, 0.0f, pos.z);

    for (int i = 0; i < 4; i++)
    {
        if (i == 1 || i == 3)
        {
            TVec3f dist;
            dist.sub(posXZ, _10[i]);

            TVec3f res;
            res.cross(dist, _40[i]);
            if (res.y < 0.0f)
            {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

// Unused function but uses stAbs(and this is the first TU that uninlines it)
f32 Course::Sector::calcXZVecCrossRate(const TVec3f &vec1, const TVec3f &vec2)
{
    return stAbs(vec1.x);
}

void Course::Area::reset()
{
    if (mData == nullptr)
        return;

    mPos.set(mData->pos);
    mZNorm.set(mData->zNorm.x * 0.0001f, mData->zNorm.y * 0.0001f, mData->zNorm.z * 0.0001f);
    mYNorm.set(mData->yNorm.x * 0.0001f, mData->yNorm.y * 0.0001f, mData->yNorm.z * 0.0001f);

    mScale.set(mData->scale);
    mScale.x *= 50.0f;
    mScale.y *= 100.0f;
    mScale.z *= 50.0f;

    mXNorm.cross(mYNorm, mZNorm);
    mFeather = mData->feather2 * 0.01f;
    mFeatherInv = 1.0f - (mData->feather1 * 0.01f);

    if (mData->echoScale != 0)
        mEchoScale = mData->echoScale * 0.01f;
    else
        mEchoScale = 1.0f;
}

// probably used in Course::Area::check(const JGeometry::TVec3f &)?
#ifdef DEBUG
CW_FORCE_STRINGS(Course_cpp, "RATE %3.2f\n(%5.1f,%5.1f,%5.1f)->(%5.1f,%5.1f,%5.1f)")
#endif

bool Course::Area::check(TVec3f *areaCoord, const TVec3f &worldPos)
{
    TVec3f diff;
    diff.sub(worldPos, mPos);

    areaCoord->x = diff.dot(mXNorm);
    areaCoord->y = diff.dot(mYNorm);
    areaCoord->z = diff.dot(mZNorm);

    switch (mData->shape)
    {
    case 1: {
        f32 lenXZ = (areaCoord->x * areaCoord->x) + (areaCoord->z * areaCoord->z);
        if (lenXZ > mScale.x * mScale.x)
            return false;
        if (areaCoord->y < -100.0f || areaCoord->y > mScale.y)
            return false;
        break;
    }
    default:
        if ((areaCoord->x < -mScale.x) || (areaCoord->x > mScale.x))
            return false;
        if (areaCoord->y < -100.0f || areaCoord->y > mScale.y)
            return false;
        if ((areaCoord->z < -mScale.z) || (areaCoord->z > mScale.z))
            return false;
        break;
    }

    return true;
}

f32 Course::Area::getRate(const TVec3f &areaCoord)
{
    f32 rate = 0.0f;
    switch (mData->shape)
    {
    case 1:
    {
        f32 lenXZ = (areaCoord.x * areaCoord.x + areaCoord.z * areaCoord.z);
        f32 xx = mScale.x * mScale.x;

        if (0.0f == xx)
            return 0.0f;
        
        f32 xz = JMAFastSqrt(lenXZ) / JMAFastSqrt(xx);

        rate = 1.0f;
        if (xz < 1.0f - mFeather)
            return 1.0f;

        if (xz <= rate)
            rate = (1.0f - xz) / mFeather;
        else
            rate = 0.0f;
        break;
    }
    default:
    {
        if (areaCoord.z >= -mScale.z && areaCoord.z <= mScale.z)
        {
            f32 feather = (areaCoord.z + mScale.z) / (2.0f * mScale.z);
            if (feather < mFeather)
                rate = feather / mFeather;
            else if (feather > mFeatherInv)
                rate = (1.0f - feather) / (1.0f - mFeatherInv);
            else
                rate = 1.0f;
        }
        break;
    }
    }
    return rate;
}

void Course::Area::drawDebugModel(u32 cameraNo)
{
    TVec3f areaPos(mPos);
    TPos3f direction;
    direction.zeroTrans();
    direction.setXYZDir(mXNorm, mYNorm, mZNorm);
    TVec3f areaScale;
    Mtx rot;
    Mtx posMtx;
    MtxPtr camMtx = RCMGetCamera(cameraNo)->GetMtx();

    switch (getShape())
    {
    case 1:
    {
        Mtx r_mtx;
        areaScale.x = mScale.x;
        areaScale.y = mScale.x;
        areaScale.z = mScale.y;

        PSMTXRotRad(r_mtx, 'X', MTXDegToRad(90.0f));
        PSMTXConcat(direction, r_mtx, direction);
        PSMTXScale(rot, areaScale.x, areaScale.y, areaScale.z); // bug? nothing is assigned to rot yet
        PSMTXConcat(direction, rot, posMtx);
        posMtx[0][3] = areaPos.x;
        posMtx[1][3] = areaPos.y;
        posMtx[2][3] = areaPos.z;
        PSMTXConcat(camMtx, posMtx, posMtx);

        GXLoadPosMtxImm(posMtx, 0);
        GXDrawCylinder(16);
        break;
    }

    default:
    {
        areaPos.scaleAdd(mScale.y * 0.5f, mYNorm, areaPos);
        f32 rad = std::sqrtf(3.0f) * 0.5f;
        areaScale.x = 2.0f * mScale.x * rad;
        areaScale.y = mScale.y * rad;
        areaScale.z = 2.0f * mScale.z * rad;
        PSMTXScale(rot, areaScale.x, areaScale.y, areaScale.z);
        PSMTXConcat(direction, rot, posMtx);
        posMtx[0][3] = areaPos.x;
        posMtx[1][3] = areaPos.y;
        posMtx[2][3] = areaPos.z;
        PSMTXConcat(camMtx, posMtx, posMtx);
        GXLoadPosMtxImm(posMtx, 0);
        GXDrawCube();
        break;
    }
    }
}

bool Course::searchAppendGround(const TVec3f &pos)
{
    bool ret = false;
    switch (getCourseID())
    case MINI_5:
        if (pos.y >= 5760.0f)
        {
            ret = true;
            _530 = 10;
            if (pos.z >= 0.0f)
            {
                if (pos.x >= 0.0f)
                    _534 = 3;
                else
                    _534 = 2;
            }
            else
            {
                if (pos.x >= 0.0f)
                    _534 = 4;
                else
                {
                    _534 = 1;
                }
            }
            _538 = 6000.0f;
        }

    return ret;
}
