#include "Osako/PadRecorder.h"

// Maybe PadRecorder inherits from some class? not that it matters, this matches as well
PadRecorder::PadRecorder() : mState(State_None)
{
    for (int i = 0; i < NUM_PLAYERPADS; i++)
    {
        mRecord[i] = nullptr;
        mButton[i] = gpaKartPad[i]->mButtons;
        mButton[i].mButton = 0; // Inline?
        mButton[i].mRepeatTimer = 0;
        mButton[i].mRepeatLastButton = 0;
    }

    for (int i = 0; i < NUM_GHOSTPADS; i++)
        mGhostRecord[i] = nullptr;
}

PadRecorder::~PadRecorder()
{
    for (int i = 0; i < NUM_PLAYERPADS; i++)
        delete mRecord[i];
    for (int i = 0; i < NUM_GHOSTPADS; i++)
        delete mGhostRecord[i];
}

void PadRecorder::setRecord(int no, KartPadRecord *record)
{
#line 68
    JUT_MINMAX_ASSERT(0, no, NUM_PLAYERPADS);
    mRecord[no] = record;
}

KartPadRecord *PadRecorder::getRecord(int no)
{
#line 78
    JUT_MINMAX_ASSERT(0, no, NUM_PLAYERPADS);
    return mRecord[no];
}

void PadRecorder::setGhostPad(int no, KartGamePad *ghostPad)
{
#line 88
    JUT_MINMAX_ASSERT(0, no, NUM_GHOSTPADS);
    mGhostPad[no] = ghostPad;
}

void PadRecorder::setGhostRecord(int no, KartPadRecord *record)
{
#line 98
    JUT_MINMAX_ASSERT(0, no, NUM_GHOSTPADS);
    mGhostRecord[no] = record;
}

KartPadRecord *PadRecorder::getGhostRecord(u8 no)
{
#line 108
    JUT_MINMAX_ASSERT(0, no, NUM_GHOSTPADS);
    return mGhostRecord[no];
}

void PadRecorder::play() { mState = State_PlayBack; }

void PadRecorder::standby()
{
    for (int i = 0; i < NUM_PLAYERPADS; i++)
    {
        mButton[i] = gpaKartPad[i]->mButtons;
        mButton[i].mButton = 0; // Inline?
        mButton[i].mRepeatTimer = 0;
        mButton[i].mRepeatLastButton = 0;
    }
}

void PadRecorder::rec() { mState = State_Recording; }

void PadRecorder::stop() { mState = State_Stopped; }

void PadRecorder::finalizeRecord(int no, RaceTime time)
{
    if (no >= NUM_PLAYERPADS)
        return;

    if (mRecord[no])
    {
        mRecord[no]->setFrame(mFrame - 1);
        mRecord[no]->getTARecord().setRaceTime(time);
    }
}

void PadRecorder::initRecord()
{
    for (int i = 0; i < NUM_PLAYERPADS; i++)
    {
        if (mRecord[i])
            mRecord[i]->setFrame(0);
    }
}

void PadRecorder::line(u8 no, KartPadData *padData)
{
    if (no >= NUM_PLAYERPADS)
        return;

    if (mRecord[no] == nullptr)
        return;
    
    switch (mState)
    {
    case State_PlayBack:
        gpaKartPad[no]->mButtons = mButton[no];
        mRecord[no]->getPadData(mFrame, padData);
        break;
    case State_Recording:
        gpaKartPad[no]->mButtons = mButton[no];
        mPadData[no] = *padData;
        break;
    }
}

void PadRecorder::playGhost()
{
    for (int i = 0; i < NUM_GHOSTPADS; i++)
    {
        if (mGhostPad[i] && mGhostRecord[i])
        {
            KartPadData padData;
            mGhostPad[i]->mButtons = mGhostButton[i];
            mGhostRecord[i]->getPadData(mFrame, &padData);
            mGhostPad[i]->expand(padData);
        }
    }
}

void PadRecorder::framework()
{
    switch (mState)
    {
    case State_Recording:
    case State_PlayBack:
        playGhost();
        break;
    }
}

void PadRecorder::nextFrame()
{
    for (int i = 0; i < NUM_PLAYERPADS; i++)
        mButton[i] = gpaKartPad[i]->mButtons;

    for (int i = 0; i < NUM_GHOSTPADS; i++)
    {
        if (mGhostPad[i] && mGhostRecord[i])
            mGhostButton[i] = mGhostPad[i]->mButtons;
    }

    if (mState == State_Recording)
    {
        for (int i = 0; i < NUM_PLAYERPADS; i++)
        {
            if (mRecord[i])
                mRecord[i]->setPadData(mFrame, &mPadData[i]);
        }
    }

    mFrame++;
}