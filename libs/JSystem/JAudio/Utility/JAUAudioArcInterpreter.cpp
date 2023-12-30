#include "JSystem/JAudio/JAUAudioArchive.h"
#include "JSystem/JUtility/JUTAssert.h"

JAUAudioArcInterpreter::JAUAudioArcInterpreter()
{
    mBase = NULL;
    mReadPtr = NULL;
}

JAUAudioArcInterpreter::~JAUAudioArcInterpreter() {}

bool JAUAudioArcInterpreter::parse(const void *data)
{
    mBase = (const u8 *)data; // save pointer to start of .baa
    mReadPtr = mBase;
    if (readU32_() != 'AA_<') // start of audio archive
    {
        return false;
    }

    while (readCommand_())
    {
    }
    return true;
}

bool JAUAudioArcInterpreter::readCommandMore(u32 cmd) { return false; }

bool JAUAudioArcInterpreter::readCommand_()
{
    u32 cmd = readU32_();
    switch (cmd)
    {
    case '>_AA': // end of audio archive
        return false;
    case 'ws  ': // waveform 
    {
        u32 bankNum = readU32_();
        u32 startOffset = readU32_();
        u32 id = readU32_(); // maybe this is a flag instead? in mkdd it's 0 of 0xffffffff(see JAUSection::loadWaveArc)
        readWS(bankNum, getContent_(startOffset), id);
        break;
    }
    case 'bnk ': // bank
    {
        u32 bankNum = readU32_();
        u32 startOffset = readU32_();
        readBNK(bankNum, getContent_(startOffset));
        break;
    }
    case 'bl_<': // bank list begin
    {
        u32 startOffset = readU32_();
        u32 cnt = readU32_();
        beginBNKList(startOffset, cnt);
        break;
    }
    case '>_bl': // bank list end
        endBNKList();
        break;
    case 'bsc ': // sequence collection?
    {
        u32 startOffset = readU32_();
        u32 endOffset = readU32_();
        readBSC(getContent_(startOffset), endOffset - startOffset);
        break;
    }
    case 'bst ': // sound table
    {
        u32 startOffset = readU32_();
        u32 endOffset = readU32_();
        readBST(getContent_(startOffset), endOffset - startOffset);
        break;
    }
    case 'bstn': // sound name table
    {
        u32 startOffset = readU32_();
        u32 endOffset = readU32_();
        readBSTN(getContent_(startOffset), endOffset - startOffset);
        break;
    }
    case 'bms ': // music sequence?
    {
        u32 soundId = readU32_();
        u32 startOffset = readU32_();
        u32 endOffset = readU32_();
        readBMS(soundId, getContent_(startOffset), endOffset - startOffset);
        break;
    }
    case 'bmsa': // music sequence archive?
    {
        u32 var1 = readU32_();
        readBMS_fromArchive(var1);
        break;
    }
    case 'dsqb': // dynamic sequence block
    {
        u32 size = readU32_();
        newDynamicSeqBlock(size);
        break;
    }
    case 'bsft': // stream file name table
    {
        u32 startOffset = readU32_();
        readBSFT(getContent_(startOffset));
        break;
    }
    case 'sect':
    {
        readU8_();
        u8 categoryIndex = readU8_();
        u8 activeSE = readU8_();
        u8 inactiveSE = readU8_();
        readMaxSeCategory(categoryIndex, activeSE, inactiveSE);
        break;
    }
    default:
        if (!readCommandMore(cmd))
        {
#line 138
            JUT_ASSERT(false);
        }
        break;
    }
    return true;
}