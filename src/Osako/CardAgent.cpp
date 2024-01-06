#include "Osako/CardAgent.h"
#include "Osako/CardMgr.h"
#include "Osako/ResetAgent.h"
#include "Osako/SystemRecord.h"

// TODO: documentation, inline rework

namespace CardAgent
{
    MessageTimer msMessageTimer;         // 0x80400cc0
    Command msCommand;                   // 0x804169d0
    int msState;                         // 0x804169d4
    int msSelectAt;                      // 0x804169d8
    int msButtonAt;                      // 0x804169dc
    int msMessageAt;                     // 0x804169e0
    int msResult;                        // 0x804169e4
    s32 msChan;                          // 0x804169e8
    SaveFile *mspSaveFile;               // 0x804169ec
    SystemFile *mspSystemFile;           // 0x804169f0
    GhostFile *mspGhostFile;             // 0x804169f4
    u8 msFlags;                          // 0x804169f8
    PrintMemoryCard *mspPrintMemoryCard; // 0x804169fc

    void create(JKRHeap *heap)
    {
        mspPrintMemoryCard = new PrintMemoryCard(heap);
        gSystemRecord.init();
    }

    void ask(Command cmd, s32 chan)
    {
        if (ResetAgent::msState != 0)
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
            msSelectAt = 14;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstSave);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand6:
            msState = 4;
            msSelectAt = 15;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstOverwrite);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand4:
            msState = 4;
            msSelectAt = 13;
            mspPrintMemoryCard->init(PrintMemoryCard::mcGstLoad);
            mspSaveFile = &gGhostFile;
            mspGhostFile = &gGhostFile;
            break;
        case mcCommand7:
            msState = 4;
            msSelectAt = 9;
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
        if (FLAG_OFF(msFlags, 4) || FLAG_OFF(msFlags, 8))
        {
            if (FLAG_OFF(msFlags, 4))
            {
                mspPrintMemoryCard->closeWindowNoSe();
            }
            msState = 0xe;
            return;
        }

        if (mspPrintMemoryCard->get_14Thing() == 0)
        {
            switch (msButtonAt)
            {
            case 1:
            case 2:
                msState = 1;
                break;
            case 3:
            case 4:
            case 5:
                msState = 4;
                msSelectAt = 4;
                msResult = 1;
                mspPrintMemoryCard->init(PrintMemoryCard::mcBrokenNoSaveContinue);
                break;
            case 6:
            case 7:
            case 8:
                msState = 0xe;
                msResult = 1;
                mspPrintMemoryCard->closeWindow();
                break;
            default:
#line 285
                JUT_PANIC(0)
                break;
            }
        }
    }

    void waitSelect()
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

        // I'm losing my mind here, so many switches inside one
        switch (CardMgr::msaCardData[msChan].mProbeStatus)
        {
        case CARD_RESULT_NOCARD:
            switch (msSelectAt)
            {
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
            case 11:
            case 12:
                msState = 4;
                msSelectAt = 1;
                mspPrintMemoryCard->init(PrintMemoryCard::mcNoCardContinue);
                return;
            case 13:
            case 14:
            case 15:
                msResult = 2;
                msState = 0xe;
                mspPrintMemoryCard->closeWindow();
                return;
            }
        default:
            if (msSelectAt == 9 && (CardMgr::probeStatusOk(0) || CardMgr::probeStatusOk(1)))
            {
                msResult = 2;
                msState = 0xe;
                mspPrintMemoryCard->closeWindow();
                break;
            }
            switch (mspPrintMemoryCard->get_14Thing())
            {
            case 0:
                switch (msSelectAt)
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 6:
                case 8:
                    msFlags &= ~1;
                    msState = 14;
                    break;
                case 5:
                    msSelectAt = 7;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcFormatLost);
                    break;
                case 7:
                    CardMgr::format(msChan);
                    mspPrintMemoryCard->init(PrintMemoryCard::mcFormatNoTouch);
                    msState = 7;
                    break;
                case 9:
                    msResult = 0;
                    msState = 0xe;
                    break;
                case 10:
                    gSystemRecord.init();
                    mspSystemFile->init();
                    CardMgr::createFile(msChan, mspSystemFile);
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMakeFileNoTouch);
                    msState = 8;
                    msMessageTimer.set();
                    break;
                case 11:
                case 12: // Some sort of fatal error?
                    ResetAgent::reset(true);
                    break;
                case 13:
                    msState = 1;
                    break;
                case 14:
                    msState = 1;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcGetSavingNoTouch);
                    msMessageTimer.set();
                    break;
                case 15:
                    msState = 1;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcGetSavingNoTouch);
                    msMessageTimer.set();
                    break;
                default:
#line 409
                    JUT_PANIC(0)
                    break;
                }
                break;
            case 1:
                switch (msSelectAt)
                {
                case 1:
                case 2:
                    msState = 3;
                    msButtonAt = 1;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcInCardPushButton);
                    break;
                case 3:
                case 4:
                case 6:
                case 8:
                    msState = 3;
                    msButtonAt = 2;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcChangeCardPushButton);
                    break;
                case 5:
                case 7:
                case 10:
                case 11:
                case 12:
                    msSelectAt = 8;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcNoSaveContinue);
                    break;
                case 9:
                    msResult = 2;
                    msState = 0xe;
                    break;
                case 13:
                case 14:
                case 15:
                    msResult = 2;
                    msState = 0xe;
                    break;
                default:
                    JUT_PANIC(0)
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    void check0()
    {
        if (FLAG_OFF(msFlags, 4))
        {
            msState = 0xe;
            mspPrintMemoryCard->closeWindowNoSe();
            return;
        }

        switch (CardMgr::msaCardData[msChan].mProbeStatus)
        {
        case CARD_RESULT_READY:
        {
            if (CardMgr::msaCardData[msChan].mSectorSize == 0x2000)
            {
                if (CardMgr::mount(msChan))
                {
                    msState = 5;
                    return;
                }
                errorIOError();
                return;
            }
            msState = 4;
            msSelectAt = 3;
            mspPrintMemoryCard->init(PrintMemoryCard::mcWithouMKContinue);
            break;
        }
        case CARD_RESULT_BUSY:
            break;
        case CARD_RESULT_NOCARD:
            errorNoCard();
            break;
        case CARD_RESULT_WRONGDEVICE:
            errorWrongDevice();
            break;
        case CARD_RESULT_FATAL_ERROR:
#line 508
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        default:
            JUT_PANIC(0);
            break;
        }
    }

    void waitMount()
    {
        // TODO: it seems like this is some sort of inline but i can't really figure out what it could be
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        if (FLAG_OFF(msFlags, 4))
        {
            mspPrintMemoryCard->closeWindowNoSe();
            msState = 0xe;
            return;
        }

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
        case CARD_RESULT_BROKEN:
            CardMgr::check(msChan);
            msState = 6;
            break;
        case CARD_RESULT_BUSY:
        {
            if (!CardMgr::mount(msChan))
                errorIOError();
            break;
        }
        case CARD_RESULT_ENCODING:
            errorEncoding();
            break;
        case CARD_RESULT_FATAL_ERROR:
#line 556
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_IOERROR:
            errorIOError();
            break;
        case CARD_RESULT_NOCARD:
            errorNoCard();
            break;
        case CARD_RESULT_WRONGDEVICE:
            errorWrongDevice();
            break;
        default:
#line 570
            JUT_PANIC(0);
            break;
        }
    }

    void waitCheck()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        if (FLAG_OFF(msFlags, 4))
        {
            mspPrintMemoryCard->closeWindowNoSe();
            msState = 0xe;
            return;
        }

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
        {
            switch (msCommand)
            {
            case mcCommand2:
            case mcCommand3:
            case mcCommand4:
            case mcCommand6:
                open();
                break;
            case mcCommand5:
                checkFree();
                break;
            }
            break;
        }
        case CARD_RESULT_BROKEN:
        {
            if (FLAG_OFF(CardMgr::msaCardData[msChan].mProcessFlag, 8))
            {
                errorIOError();
                break;
            }
        }
        case CARD_RESULT_ENCODING:
            errorEncoding();
            break;
        case CARD_RESULT_FATAL_ERROR:
#line 627
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_IOERROR:
            errorIOError();
            break;
        case CARD_RESULT_NOCARD:
            errorNoCard();
            break;
        case CARD_RESULT_WRONGDEVICE:
            errorWrongDevice();
            break;
        case CARD_RESULT_BUSY:
            CardMgr::check(msChan);
            break;
        default:
#line 646
            JUT_PANIC(0);
        }
    }

    void waitFormat()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        if (FLAG_OFF(msFlags, 4))
        {
            mspPrintMemoryCard->closeWindowNoSe();
            msState = 0xe;
            return;
        }

        switch (CardMgr::msaCardData[msChan].mCardStatus)
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
            msSelectAt = 11;
            mspPrintMemoryCard->init(PrintMemoryCard::mcNoSpaceGoIPL);
            return;
        }

        if (CardMgr::msaCardData[msChan].mFilesNotUsed > 0)
        {
            msState = 4;
            switch (msCommand)
            {
            case mcCommand2:
                msSelectAt = 10;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMakeFile);
                break;
            case mcCommand5:
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
        msSelectAt = 12;
        mspPrintMemoryCard->init(PrintMemoryCard::mcTooManyFilesGoIPL);
    }

    void open()
    {
        switch (CardMgr::openFile(msChan, mspSaveFile))
        {
        case CARD_RESULT_READY:
            switch (msCommand)
            {
            case mcCommand2:
            case mcCommand3:
            case mcCommand4:
                CardMgr::read(msChan, SaveFile::mcHeader);
                msState = 10;
                break;
            case mcCommand6:
                CardMgr::deleteFile(msChan);
                msState = 0xc;
                break;
            default:
#line 763
                JUT_PANIC(0);
                break;
            }
            break;
        case CARD_RESULT_NOFILE:
            switch (msCommand)
            {
            case mcCommand2:
                checkFree();
                break;
            case mcCommand3:
                msState = 4;
                msSelectAt = 6;
                mspPrintMemoryCard->init(PrintMemoryCard::mcNoFileNoSave);
                break;
            case mcCommand4:
                msState = 3;
                msButtonAt = 8;
                mspPrintMemoryCard->init(PrintMemoryCard::mcGstNoLoad);
                break;
            default:
#line 787
                JUT_PANIC(0)
                break;
            }
            break;
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOPERM:
        case CARD_RESULT_FATAL_ERROR:
#line 795
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_NOCARD:
            errorNoCard();
            break;
        case CARD_RESULT_BROKEN:
            errorEncoding();
            break;
        default:
#line 806
            JUT_PANIC(0)
            break;
        }
    }

    void waitCreate()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
            CardMgr::write(msChan, SaveFile::mcHeader);
            msState = 9;
            break;
        case CARD_RESULT_NOENT:
        case CARD_RESULT_INSSPACE:
        case CARD_RESULT_NAMETOOLONG:
        case CARD_RESULT_FATAL_ERROR:
#line 834
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_EXIST:
        {
            switch (msCommand)
            {
            case mcCommand5:
            case mcCommand6:
                mspGhostFile->remakeFileName();
                CardMgr::createFile(msChan, mspSaveFile);
                break;
            default:
#line 847
                JUT_PANIC("create")
                break;
            }
        }
        break;
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOCARD:
        case CARD_RESULT_IOERROR:
            switch (msCommand)
            {
            case mcCommand2:
                msState = 3;
                msButtonAt = 4;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissMakeFile);
                break;
            case mcCommand3:
                msState = 3;
                msButtonAt = 5;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                break;
            case mcCommand5:
            case mcCommand6:
                msState = 3;
                msButtonAt = 7;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                break;
            default:
#line 874
                JUT_PANIC("create")
                break;
            }
            break;
        default:
            JUT_PANIC(0);
            break;
        }
    }

    void waitWrite()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
        {
            switch (msCommand)
            {
            case mcCommand3:
                mspSystemFile->setActiveChecksum();
            case mcCommand2:
            case mcCommand5:
            case mcCommand6:
                CardMgr::setStatus(msChan);
                msState = 0xd;
                break;
            default:
#line 915
                JUT_PANIC(0);
                break;
            }
            break;
        }
        case CARD_RESULT_FATAL_ERROR:
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOCARD:
        case CARD_RESULT_NOFILE:
        case CARD_RESULT_IOERROR:
        case CARD_RESULT_NOPERM:
        case CARD_RESULT_LIMIT:
        case CARD_RESULT_CANCELED:
        {
            switch (msCommand)
            {
            case mcCommand2:
                msState = 3;
                msButtonAt = 4;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissMakeFile);
                break;
            case mcCommand3:
                msState = 3;
                msButtonAt = 5;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                break;
            case mcCommand5:
            case mcCommand6:
                msState = 3;
                msButtonAt = 7;
                mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                break;
            default:
#line 949
                JUT_PANIC(0);
            }
            msResult = 1;
            break;
        }
        default:
            JUT_PANIC(0);
            break;
        }
    }

    void waitRead()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
            mspSaveFile->checkData();
            switch (msCommand)
            {
            case mcCommand2:
                msFlags |= 1;
                if (mspSystemFile->_602c == 0 || CardMgr::areOffsetsOk(msChan))
                {
                    msState = 3;
                    msButtonAt = 6;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcBrokenNoLoad);
                }
                else
                {
                    mspSystemFile->setDataChecksums();
                    mspSystemFile->store();
                    msState = 0xe;
                }
                break;
            case mcCommand3:
                if (mspSystemFile->isSavable() || CardMgr::areOffsetsOk(msChan))
                {
                    if (FLAG_ON(mspSystemFile->_6023, 4))
                        CardMgr::write(msChan, SaveFile::mcHeader);
                    else if (mspSystemFile->_602c == 0)
                        CardMgr::write(msChan, SaveFile::mcData);
                    else
                        CardMgr::write(msChan, SaveFile::mcDataSub);
                    msState = 9;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcGameSavingNoTouch);
                    msMessageTimer.set();
                    break;
                }

                msState = 4;
                msSelectAt = 6;
                mspPrintMemoryCard->init(PrintMemoryCard::mcNoFileNoSave);
                break;

                break;
            case mcCommand4:
                switch (mspGhostFile->mGhostType)
                {
                case 2:
                    msState = 2;
                    msMessageTimer.set();
                    mspPrintMemoryCard->init(PrintMemoryCard::mcLoaded);
                    msMessageAt = 3;

                    break;
                case 1:
                    msState = 3;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcGstBrokenNoLoad);
                    msButtonAt = 8;
                    break;
                default:
                    break;
                }
                break;
            default:
#line 1045
                JUT_PANIC(0)
                break;
            }
            break;
        case CARD_RESULT_LIMIT:
        case CARD_RESULT_CANCELED:
        case CARD_RESULT_FATAL_ERROR:
#line 1053
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_NOCARD:
            errorNoCard();
            break;
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOFILE:
        case CARD_RESULT_NOPERM:
            errorIOError();
            break;
        default:
#line 1065
            JUT_PANIC(0)
            break;
        }
    }

    void waitRename()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
        {
            CardMgr::setStatus(msChan);
            msState = 0xd;
            break;
        }
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOCARD:
        case CARD_RESULT_IOERROR:
            msState = 3;
            msButtonAt = 5;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;
        case CARD_RESULT_NOENT:
        case CARD_RESULT_INSSPACE:
        case CARD_RESULT_NAMETOOLONG:
        case CARD_RESULT_FATAL_ERROR:
#line 1105
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        case CARD_RESULT_EXIST:
        {
            if (msCommand == mcCommand6)
            {
                mspGhostFile->remakeFileName();
                CardMgr::renameFile(msChan);
                return;
            }
#line 1118
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        }
        default:
            JUT_PANIC(0);
            break;
        }
    }

    void waitDelete()
    {
        s32 chan = msChan;
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[chan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

        if (taskStatus == CardMgr::mcTaskDone)
            CardMgr::msaCardData[chan].mTaskStatus = CardMgr::mcNoTask;

        switch (CardMgr::msaCardData[msChan].mCardStatus)
        {
        case CARD_RESULT_READY:
        {
            CardMgr::createFile(msChan, mspGhostFile);
            msState = 8;
            msMessageTimer.set();
            break;
        }
        case CARD_RESULT_BUSY:
        case CARD_RESULT_NOCARD:
        case CARD_RESULT_NOFILE:
        case CARD_RESULT_IOERROR:
        case CARD_RESULT_NOPERM:
            msState = 3;
            msButtonAt = 7;
            mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
            break;

        case CARD_RESULT_FATAL_ERROR:
#line 1159
            JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
            break;
        default:
            JUT_PANIC(0);
            break;
        }
    }

    void waitSetStatus()
    {
        CardMgr::TaskStatus taskStatus = CardMgr::msaCardData[msChan].mTaskStatus;
        if (taskStatus != CardMgr::mcTaskDone)
            return;

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

        if (waitTime > 1.0f)
        {
            CardMgr::resetTaskStatus(msChan);

            switch (CardMgr::msaCardData[msChan].mCardStatus)
            {
            case CARD_RESULT_READY:
                msState = 2;
                msMessageTimer.set();
                switch (msCommand)
                {
                case mcCommand2:
                    msMessageAt = 1;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMadeFile);
                    mspSystemFile->setDataChecksums();
                    msFlags |= 1;
                    break;
                case mcCommand5:
                case mcCommand6:
                    mspGhostFile->mGhostType = 4; // hmm, ig this needs to be renamed
                case mcCommand3:
                    msMessageAt = 2;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcSaved);
                    break;
                default:
#line 1202
                    JUT_PANIC(0)
                    break;
                }
                break;
            case CARD_RESULT_FATAL_ERROR:
#line 1208
                JUT_ASSERT_MSG(0, "Program Error at CardAgent!\n")
                break;
            case CARD_RESULT_BUSY:
            case CARD_RESULT_NOCARD:
            case CARD_RESULT_NOFILE:
            case CARD_RESULT_IOERROR:
            case CARD_RESULT_NOPERM:
                msState = 3;
                switch (msCommand)
                {
                case mcCommand2:
                    msButtonAt = 4;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMissMakeFile);
                    break;
                case mcCommand3:
                    msButtonAt = 5;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                case mcCommand5:
                    msButtonAt = 7;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                case mcCommand6:
                    msButtonAt = 8;
                    mspPrintMemoryCard->init(PrintMemoryCard::mcMissSaved);
                    break;
                default:
                    JUT_PANIC(0)
                    break;
                }
                break;
            default:
#line 1238
                JUT_PANIC(0)
                break;
            }
        }
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