#include "Kaneshige/SimpleDrawer.h"

SimpleDrawer *SimpleDrawer::sDrawingDrawer;
bool SimpleDrawer::sEnableCheckHidding = true;

SimpleDrawer::SimpleDrawer()
{
    mKeepDrawMat = false;
    mDrawingMaterial = nullptr;
    mPhase = 0;
    mIsMatDrawOn = false;
}

void SimpleDrawer::drawInit(ExModel *model)
{
    J3DModelData *modelData = model->getModelData(0);
    drawInit(modelData);
}

void SimpleDrawer::drawInit(J3DModelData *modelData)
{
    sDrawingDrawer = this;
    modelData->syncJ3DSys();
    J3DShape::resetVcdVatCache();
    J3DJoint *joint = modelData->getJointNodePointer(0);

    mPhase = 0;
    mKeepDrawMat = true;
    mDrawingMaterial = joint->getMesh();
    mIsMatDrawOn = false;
}

bool SimpleDrawer::loadPreDrawSetting()
{
    bool ret = true;

    switch (mPhase)
    {
    case 0:
    {
        if (mDrawingMaterial)
        {
            if (mIsMatDrawOn || !sEnableCheckHidding)
            {
                mPhase = 1;
                mDrawingMaterial->loadSharedDL();
                mDrawingMaterial->getShape()->loadPreDrawSetting();
                break;
            }

            if (mKeepDrawMat == false)
            {
                mDrawingMaterial = mDrawingMaterial->getNext();
                mIsMatDrawOn = false;
            }
            mKeepDrawMat = false;
            break;
        }

        ret = false;
        sDrawingDrawer = nullptr;
        break;
    }
    case 1:
    {
        mPhase = 0;
        mDrawingMaterial = mDrawingMaterial->getNext();
        mIsMatDrawOn = false;
        break;
    }
    }

    return ret;
}