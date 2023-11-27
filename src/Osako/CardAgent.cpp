#include "Osako/CardAgent.h"
#include "Osako/CardMgr.h"
#include "Osako/clock.h"
#include "Osako/SystemRecord.h"

// WIP

namespace CardAgent
{
    PrintMemoryCard *mspPrintMemoryCard;

    void create(JKRHeap *heap)
    {
        mspPrintMemoryCard = new PrintMemoryCard(heap);
        gSystemRecord.init();
    }

    void ask(Command cmd, s32 chan)
    {
        if (msState != 0)
        {
            msState = 0;
            msResult = 1;
            return;
        }

        msCommand = cmd;
        msChan = chan;
        msState = 0xe;
        msResult = 0;
        msFlags &= ~8;

        mspPrintMemoryCard->init(PrintMemoryCard::mcMsg40);

        switch (cmd)
        {
        case mcCommand3:
            if (FLAG_OFF(msFlags, 1))
            {
                msState = 1;
                mspSystemFile = &gSystemFile;
                mspSaveFile = &gSystemFile;
            }
            break;
        case mcCommand1:
            if (FLAG_ON(msFlags, 2))
            {
                msState = 1;
                mspSystemFile = &gSystemFile;
                mspSaveFile = &gSystemFile;
                msFlags |= 8;
                msCommand = mcCommand2;
            }
            break;
        case mcCommand2:
            if (FLAG_ON(msFlags, 2))
            {
                msState = 1;
                mspSystemFile = &gSystemFile;
                mspSaveFile = &gSystemFile;
                msFlags |= 2;
            }
            break;
        case mcCommand5:
            msState = 4;
            msSelectAt = 0xe;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstSave);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand6:
            msState = 4;
            msSelectAt = 0xf;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstOverwrite);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand4:
            msState = 4;
            msSelectAt = 0xd;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstLoad);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand7:
            msState = 4;
            msSelectAt = 0x9;
            CardMgr::msaCardData[0].mPrevProbeStatus = CardMgr::msaCardData[0].mProbeStatus;
            CardMgr::msaCardData[1].mPrevProbeStatus = CardMgr::msaCardData[1].mProbeStatus;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoSaveContinue);
            break;
        }
    }

    void draw()
    {
        switch (msState)
        {
        case 0x0:
        case 0xf:
            break;
        default:
            if (FLAG_ON(msFlags, 8))
                mspPrintMemoryCard->draw();
            break;
        }
    }

    void calc()
    {
        switch (msState)
        {
        case 1:
            check0();
            break;
        case 2:
            waitMessage();
            break;
        case 3:
            waitButton();
            break;
        case 4:
            waitSelect();
            break;
        case 5:
            waitMount();
            break;
        case 6:
            waitCheck();
            break;
        case 7:
            waitFormat();
            break;
        case 8:
            waitCreate();
            break;
        case 9:
            waitWrite();
            break;
        case 10:
            waitRead();
            break;
        case 0xb:
            waitRename();
            break;
        case 0xc:
            waitDelete();
            break;
        case 0xd:
            waitSetStatus();
            break;
        case 0xe:
            waitQuit();
            break;
        }

        switch (msState)
        {
        case 0x0:
        case 0xf:
            break;
        default:
            if (FLAG_ON(msFlags, 8))
                mspPrintMemoryCard->calc();
            break;
        }
    }

    void waitMessage()
    {
        if (FLAG_OFF(msFlags, 4) || FLAG_OFF(msFlags, 8))
        {
            if (FLAG_OFF(msFlags, 4))
            {
                mspPrintMemoryCard->closeWindowNoSe();
            }
            msState = 0xe;
            return;
        }
        // inline?
        f32 waitTime;
        switch (msMessageTimer._c)
        {
        case 1:
            waitTime = Clock::calcFrameTime(msMessageTimer.mTime);
            break;
        case 2:
            waitTime = msMessageTimer._8;
            break;
        default:
            waitTime = 0.0f;
            break;
        }

        if (waitTime > 2.0f)
        {
            switch (msMessageAt)
            {
            case 1:
            case 2:
            case 3:
                msState = 0xe;
                msResult = 0;
                mspPrintMemoryCard->closeWindow();
                break;
            default:
#line 238
                JUT_PANIC(0);
            }
        }
    }

    void waitButton()
    {
    }

    void waitSelect()
    {
    }

    void check0()
    {
    }

    void waitMount()
    {
    }

    void waitCheck()
    {
    }

    void waitFormat()
    {
        s32 chan = msChan;
        if (CardMgr::msaCardData[chan].mTaskStatus != CardMgr::mcTaskDone)
            return;

        if (CardMgr::msaCardData[chan].mTaskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        if (FLAG_OFF(msFlags, 4))
        {
            mspPrintMemoryCard->closeWindowNoSe();
            msState = 0xe;
            return;
        }

        switch (CardMgr::msaCardData[chan].mCardStatus)
        {
        case CARD_RESULT_READY:
            msState = 1;
            break;
        case CARD_RESULT_FATAL_ERROR:
#line 678
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOCARD:
        case CARD_RESULT_IOERROR:
            msState = 3;
            msButtonAt = 3;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissFormat);
            break;
        default:
#line 689
            JUT_PANIC(0);
            break;
        }
    }

    void checkFree()
    {
        s32 byteNotUsed = CardMgr::msaCardData[msChan].mByteNotUsed;
        u32 fileSize = mspSaveFile->getFileSize();
        if (byteNotUsed < fileSize)
        {
            msState = 4;
            msSelectAt = 0xb;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoSpaceGoIPL);
            return;
        }

        if (CardMgr::msaCardData[msChan].mFilesNotUsed > 0)
        {
            msState = 4;
            switch (msCommand)
            {
            case 2:
                msSelectAt = 0xa;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMakeFile);
                break;
            case 5:
                CardMgr::createFile(msChan, mspSaveFile);
                msState = 8;
                break;
            default:
#line 726
                JUT_PANIC("checkFree");
                break;
            }
            return;
        }

        msState = 4;
        msSelectAt = 0xc;
        mspPrintMemoryCard->init(PrintMemoryCard::mcTooManyFilesGoIPL);
    }

    void open()
    {
    }

    void waitCreate()
    {
    }

    void waitWrite()
    {
    }

    void waitRead()
    {
    }

    void waitRename()
    {
    }

    void waitDelete()
    {
    }

    void waitSetStatus()
    {
    }

    void waitQuit()
    {
        if (mspPrintMemoryCard->get_1cThing() == 3 || FLAG_OFF(msFlags, 8))
        {
            mspPrintMemoryCard->reset();
            CardMgr::closeFile(msChan);
            CardMgr::unmount(msChan);

            if (FLAG_OFF(msFlags, 4))
            {
                msFlags &= ~0x4;
                msState = 0xf;
            }
            else
            {
                msState = 0;
            }
        }
    }

    void reset()
    {
        msFlags &= ~0x1;
        msFlags &= ~0x2;
        msFlags &= ~0x4;
        msState = 0;
    }

    bool isGhostSavable()
    {
        for (s32 chan = 0; chan < CARD_NUM_CHANS; chan++)
        {
            if (CardMgr::msaCardData[chan].canWrite())
                return true;
        }
        return false;
    }

    void errorNoCard()
    {
        switch (msCommand)
        {
        case mcCommand2:
        case mcCommand3:
            msState = 4;
            msSelectAt = 1;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoCardContinue);
            break;
        case mcCommand4:
            msState = 3;
            msButtonAt = 8;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstNoLoad);
            break;

        case mcCommand5:
        case mcCommand6:
            msState = 3;
            msButtonAt = 7;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;
        default:
#line 1326
            JUT_PANIC(0);
            break;
        }
    }

    void errorWrongDevice()
    {
        switch (msCommand)
        {
        case mcCommand2:
        case mcCommand3:
            msState = 4;
            msSelectAt = 2;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoUseContinue);
            break;
        case mcCommand4:
            msState = 3;
            msButtonAt = 8;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstNoLoad);
            break;

        case mcCommand5:
        case mcCommand6:
            msState = 3;
            msButtonAt = 7;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;
        default:
#line 1357
            JUT_PANIC(0);
            break;
        }
    }

    void errorIOError()
    {
        CardMgr::unmount(msChan);
        switch (msCommand)
        {
        case mcCommand2:
        case mcCommand3:
            msState = 4;
            msSelectAt = 4;
            mspPrintMemoryCard->init(PrintMemoryCard::mcBrokenNoSaveContinue);
            break;
        case mcCommand4:
            msState = 3;
            msButtonAt = 8;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstNoLoad);
            break;

        case mcCommand5:
        case mcCommand6:
            msState = 3;
            msButtonAt = 7;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;
        default:
#line 1391
            JUT_PANIC(0);
            break;
        }
    }

    void errorEncoding()
    {
        switch (msCommand)
        {
        case mcCommand3:
            msState = 4;
            msSelectAt = 6;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoFileNoSave);
            break;
        case mcCommand2:
            msState = 4;
            msSelectAt = 5;
            mspPrintMemoryCard->init(PrintMemoryCard::mcBrokenGoFormat);
            break;

        case mcCommand4:
            msState = 3;
            msButtonAt = 8;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstNoLoad);
            break;

        case mcCommand5:
        case mcCommand6:
            msState = 3;
            msButtonAt = 7;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;
        default:
#line 1426
            JUT_PANIC(0);
            break;
        }
    }
}