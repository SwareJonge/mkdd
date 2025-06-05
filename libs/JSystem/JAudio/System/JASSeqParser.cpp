#include "JSystem/JAudio/System/JASSeqParser.h"
#include "JSystem/JAudio/System/JASReport.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JSupport.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/types.h"

u16 (*JASSeqParser::sCallBackFunc)(JASTrack*, u16);

JASSeqParser::CmdInfo JASSeqParser::sCmdInfo[96] = {
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdNoteOn, 3,0},
    {&JASSeqParser::cmdNoteOff, 1,0},
    {&JASSeqParser::cmdNote, 4,0x40},
    {&JASSeqParser::cmdSetLastNote, 1,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdParamE, 2,0},
    {&JASSeqParser::cmdParamI, 2,0x4},
    {&JASSeqParser::cmdParamEI, 3,0x10},
    {&JASSeqParser::cmdParamII, 3,0x14},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdOpenTrack, 2,0x8},
    {&JASSeqParser::cmdCloseTrack, 1,0},
    {&JASSeqParser::cmdCall, 1,0x2},
    {&JASSeqParser::cmdCallF, 2,0x8},
    {&JASSeqParser::cmdRet, 0,0},
    {&JASSeqParser::cmdRetF, 1,0},
    {&JASSeqParser::cmdJmp, 1,0x2},
    {&JASSeqParser::cmdJmpF, 2,0x8},
    {&JASSeqParser::cmdJmpTable, 2,0xb},
    {&JASSeqParser::cmdCallTable, 2,0xb},
    {&JASSeqParser::cmdLoopS, 1,0x1},
    {&JASSeqParser::cmdLoopE, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdReadPort, 2,0},
    {&JASSeqParser::cmdWritePort, 2,0xc},
    {&JASSeqParser::cmdCheckPortImport, 1,0},
    {&JASSeqParser::cmdCheckPortExport, 1,0},
    {&JASSeqParser::cmdParentWritePort, 2,0xc},
    {&JASSeqParser::cmdChildWritePort, 2,0xc},
    {&JASSeqParser::cmdParentReadPort, 2,0},
    {&JASSeqParser::cmdChildReadPort, 2,0},
    {&JASSeqParser::cmdRegLoad, 2,4},
    {&JASSeqParser::cmdReg, 3,0x30},
    {&JASSeqParser::cmdReg, 3,0x10},
    {&JASSeqParser::cmdRegUni, 2,0},
    {&JASSeqParser::cmdRegTblLoad, 4,0xe0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdTempo, 1,0x1},
    {&JASSeqParser::cmdBankPrg, 1,0x1},
    {&JASSeqParser::cmdBank, 1,0},
    {&JASSeqParser::cmdPrg, 1,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdEnvSet, 2,0x8},
    {&JASSeqParser::cmdSimpleADSR, 5,0x155},
    {&JASSeqParser::cmdBusConnect, 2,0x4},
    {&JASSeqParser::cmdIIRCutOff, 1,0},
    {&JASSeqParser::cmdIIRSet, 4,0x55},
    {&JASSeqParser::cmdFIRSet, 1,0x2},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdWait, 0,0},
    {&JASSeqParser::cmdWaitByte, 1,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdSetIntTable, 1,0x2},
    {&JASSeqParser::cmdSetInterrupt, 1,0x1},
    {&JASSeqParser::cmdDisInterrupt, 1,0x1},
    {&JASSeqParser::cmdRetI, 0,0},
    {&JASSeqParser::cmdClrI, 0,0},
    {&JASSeqParser::cmdIntTimer, 2,0x4},
    {&JASSeqParser::cmdSyncCPU, 1,0x1},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {&JASSeqParser::cmdPrintf, 0,0},
    {&JASSeqParser::cmdNop, 0,0},
    {&JASSeqParser::cmdFinish, 0,0},
};

JASSeqParser::CmdInfo JASSeqParser::sExtCmdInfo[255] = {
    {NULL, 0,0},
    {&JASSeqParser::cmdDump, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0},
    {NULL, 0,0}
};

#include "JSystem/JAudio/JASFakeMatch.h"

bool JASSeqParser::conditionCheck(JASTrack *track, JASSeqParser::BranchCondition cond) {
    u16 data = readReg(track, 3);
    switch (cond) {
    case 0:
        return 1;
    case 1:
        return data == 0;
    case 2:
        return data != 0;
    case 3:
        return data == 1;
    case 4:
        return 0x8000 <= data;
    case 5:
        return 0x8000 > data;
    default:
#line 462
        JUT_WARNING_F2("%s", "Unexpected condition flag");
        return 0;
    }
}

void JASSeqParser::writeReg(JASTrack *track, u32 reg, u32 data) {
    track->writeReg(JASRegisterParam::REG3, data);
    if (reg < 0x40) {
        track->writeReg(JASRegisterParam::RegID(reg), data);
        return;
    }
    switch (reg) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
        track->setPort(reg - 0x40, data);
        break;
    case 0x60:
    case 0x61:
#line 501
        JUT_WARNING_F2("%s", "writeReg for Read-Only Register");
        break;
    case 0x62:
        track->setTimebase(data);
        break;
    case 0x63:
        track->setTranspose(data);
        break;
    case 0x64:
        track->setBendSense(data);
        break;
    case 0x65:
        track->setGateRate(data);
        break;
    case 0x66:
        track->setSkipSample(data);
        break;
    case 0x67:
        track->setBankNumber(u8(data));
        break;
    case 0x68:
        track->setProgNumber(u8(data));
        break;
    case 0x69:
        track->setPanPower(data / 32767.0f);
        break;
    case 0x6a:
        track->setReleasePrio(JSUHiByte(data));
        track->setNoteOnPrio(JSULoByte(data));
        break;
    case 0x6b:
        track->setNoteOnPrio(data);
        break;
    case 0x6c:
        track->setReleasePrio(data);
        break;
    case 0x6d:
        track->setDirectRelease(data);
        break;
    case 0x6e:
        track->setVibDepth(data / 1524.0f);
        break;
    case 0x6f:
        track->setVibDepth(data / 12192.0f);
        break;
    case 0x70:
        track->setTremDepth(data / 256.0f);
        break;
    case 0x71:
        track->setVibPitch(data / 64.0f);
        break;
    case 0x72:
        track->setTremPitch(data / 64.0f);
        break;
    case 0x73:
        track->setVibDelay(data);
        break;
    case 0x74:
        track->setTremDelay(data);
        break;
    default:

        break;
    }
}

u32 JASSeqParser::readReg(JASTrack *track, u32 reg) const {
    if (reg < 0x40) {
        return track->readReg(JASRegisterParam::RegID(reg));
    }
    u32 result = 0;
    u16 childIdxMask;
    switch (reg) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
        result = track->getPort(reg - 0x40);
        break;
    case 0x60:
        childIdxMask = 1;
        for (u32 i = 0; i < JASTrack::MAX_CHILDREN; i++) {
            JASTrack *child = track->getChild(i);
            if (child && child->getStatus() == 1) {
                result |= childIdxMask;
            }
            childIdxMask <<= 1;
        }
        break;
    case 0x61:
        result = track->getSeqCtrl()->getSeqReader().getLoopCount();
        break;
    case 0x62:
        result = track->getTimebase();
        break;
    case 0x63:
        result = track->getTranspose();
        break;
    case 0x64:
        result = track->getBendSense();
        break;
    case 0x65:
        result = track->getGateRate();
        break;
    case 0x66:
        result = track->getSkipSample();
        break;
    case 0x67:
        result = track->getBankNumber();
        break;
    case 0x68:
        result = track->getProgNumber();
        break;
    case 0x69:
        result = track->getPanPower() * 32767.0f;
        break;
    case 0x6a:
        result = track->getReleasePrio();
        result |= track->getNoteOnPrio();
        break;
    case 0x6b:
        result = track->getNoteOnPrio();
        break;
    case 0x6c:
        result = track->getReleasePrio();
        break;
    case 0x6d:
        result = track->getDirectRelease();
        break;
    case 0x6e:
        result = track->getVibDepth() * 1524.0f;
        break;
    case 0x6f:
        result = track->getVibDepth() * 12192.0f;
        break;
    case 0x70:
        result = track->getTremDepth() * 256.0f;
        break;
    case 0x71:
        result = track->getVibPitch() * 64.0f;
        break;
    case 0x72:
        result = track->getTremPitch() * 64.0f;
        break;
    case 0x73:
        result = track->getVibDelay();
        break;
    case 0x74:
        result = track->getTremDelay();
        break;
    default:
        break;
    }
    return result;
}

s32 JASSeqParser::cmdOpenTrack(JASTrack *track, u32 *pData) {
    u32 trackNo = pData[0];
    u32 offset = pData[1];
    JASTrack *child = track->openChild(trackNo);
    if (!child) {
#line 678
        JUT_WARNING_F2("%s", "Cannot open child track");
        return 0;
    }
    child->setSeqData(track->getSeqCtrl()->getBase(), offset);
    child->start();
    return 0;
}

s32 JASSeqParser::cmdCloseTrack(JASTrack *track, u32 *pData) {
    track->closeChild(*pData);
    return 0;
}

s32 JASSeqParser::cmdCall(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->call(*pData);
    return 0;
}

s32 JASSeqParser::cmdCallF(JASTrack *track, u32 *pData) {
    if (conditionCheck(track, BranchCondition(pData[0]))) {
        track->getSeqCtrl()->call(pData[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdRet(JASTrack *track, u32 *pData) {
    if (!track->getSeqCtrl()->ret()) {
#line 717
        JUT_WARNING_F2("%s", "cannot ret for call-stack is NULL");
        return 0;
    }
    return 0;
}

s32 JASSeqParser::cmdRetF(JASTrack *track, u32 *pData) {
    if (conditionCheck(track, BranchCondition(pData[0]))) {
        if (!track->getSeqCtrl()->ret()) {
#line 728
            JUT_WARNING_F2("%s", "cannot ret for call-stack is NULL");
            return 0;
        }
    }
    return 0;
}

s32 JASSeqParser::cmdJmp(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->jump(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdJmpF(JASTrack *track, u32 *pData) {
    if (conditionCheck(track, BranchCondition(pData[0]))) {
        track->getSeqCtrl()->jump(pData[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdJmpTable(JASTrack *track, u32 *pData) {
    u32 offset = pData[0];
    offset += offset * 2;
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    seqCtrl->jump(seqCtrl->get24(pData[1] + offset));
    return 0;
}

s32 JASSeqParser::cmdCallTable(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    u32 offset = pData[0];
    offset += offset  *2;
    seqCtrl->call(seqCtrl->get24(pData[1] + offset));
    return 0;
}

s32 JASSeqParser::cmdLoopS(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->loopStart(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdLoopE(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->loopEnd();
    return 0;
}

s32 JASSeqParser::cmdNote(JASTrack *track, u32 *pData) {
    execNoteOnGate(track, pData[1], pData[2], pData[3], pData[0]);
    return 0;
}

s32 JASSeqParser::cmdNoteOn(JASTrack *track, u32 *pData) {
    execNoteOnMidi(track, pData[1], pData[0], pData[2]);
    return 0;
}

s32 JASSeqParser::cmdNoteOff(JASTrack *track, u32 *pData) {
    execNoteOff(track, pData[0]);
    return 0;
}

s32 JASSeqParser::cmdReadPort(JASTrack *track, u32 *pData) {
    u16 tmp = track->readPortSelf(pData[0]);
    writeReg(track, pData[1], tmp);
    return 0;
}

s32 JASSeqParser::cmdWritePort(JASTrack *track, u32 *pData) {
    track->writePortSelf(pData[0], pData[1]);
    return 0;
}

s32 JASSeqParser::cmdParentWritePort(JASTrack *track, u32 *pData) {
    JASTrack *parent = track->getParent();
    if (parent) {
        parent->writePort(pData[0], pData[1]);
    } 
    else {
#line 834
        JUT_WARNING_F2("%s", "parent == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdChildWritePort(JASTrack *track, u32 *pData) {
    u8 p0 = pData[0];
    JASTrack *child = track->getChild(JSUHiNibble(p0));
    if (child) {
        child->writePort(JSULoNibble(p0), pData[1]);
    } 
    else {
#line 847
        JUT_WARNING_F2("%s", "child == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdParentReadPort(JASTrack *track, u32 *pData) {
    JASTrack *parent = track->getParent();
    if (parent) {
        u16 tmp = parent->readPort(pData[0]);
        writeReg(track, pData[1], tmp);
    } 
    else {
#line 863
        JUT_WARNING_F2("%s", "parent == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdChildReadPort(JASTrack *track, u32 *pData) {
    u8 p0 = pData[0];
    JASTrack *child = track->getChild(JSUHiNibble(p0));
    if (child) {
        u16 tmp = child->readPort(JSULoNibble(p0));
        writeReg(track, pData[1], tmp);
    } 
    else {
#line 879
        JUT_WARNING_F2("%s", "child == NULL");
    }
    return 0;
}

s32 JASSeqParser::cmdCheckPortImport(JASTrack *track, u32 *pData) {
    u8 tmp = track->checkPortIn(pData[0]);
    track->writeReg(JASRegisterParam::REG3, tmp);
    return 0;
}

s32 JASSeqParser::cmdCheckPortExport(JASTrack *track, u32 *pData) {
    u8 tmp = track->checkPort(pData[0]);
    track->writeReg(JASRegisterParam::REG3, tmp);
    return 0;
}

s32 JASSeqParser::cmdWait(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    seqCtrl->wait(seqCtrl->readMidiValue());
    return 0;
}

s32 JASSeqParser::cmdWaitByte(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->wait(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdSetLastNote(JASTrack *track, u32 *pData) {
    u32 key = pData[0];
    if (key < 256) {
        track->setLatestKey(key);
    } 
    else {
#line 927
        JUT_WARNING_F2("%s", "Too large latest key");
    }
    return 0;
}

s32 JASSeqParser::cmdEnvSet(JASTrack *track, u32 *pData) {
    track->setOscTable(pData[0], (JASOscillator::Point*)track->getSeqCtrl()->getAddr(pData[1]));
    return 0;
}

s32 JASSeqParser::cmdSimpleADSR(JASTrack *track, u32 *pData) {
    track->setOscAdsr(pData[0], pData[1], pData[2], pData[3], pData[4]);
    return 0;
}

s32 JASSeqParser::cmdBusConnect(JASTrack *track, u32 *pData) {
    track->connectBus(pData[0], pData[1]);
    return 0;
}

s32 JASSeqParser::cmdSetIntTable(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->setIntrTable(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdSetInterrupt(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->setIntrMask(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdDisInterrupt(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->clrIntrMask(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdClrI(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    seqCtrl->clrIntr();
    seqCtrl->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdRetI(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    seqCtrl->retIntr();
    seqCtrl->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdIntTimer(JASTrack *track, u32 *pData) {
    track->getSeqCtrl()->setIntrTimer(pData[0], pData[1]);
    return 0;
}

s32 JASSeqParser::cmdSyncCPU(JASTrack *track, u32 *pData) {
    u16 r31 = 0xffff;
    if (sCallBackFunc) {
        r31 = sCallBackFunc(track, pData[0]);
    }
    track->writeReg(JASRegisterParam::REG3, r31);
    return 0;
}

s32 JASSeqParser::cmdTempo(JASTrack *track, u32 *pData) {
    track->setTempo(pData[0]);
    return 0;
}

s32 JASSeqParser::cmdFinish(JASTrack *track, u32 *pData) {
    return -1;
}

s32 JASSeqParser::cmdNop(JASTrack *track, u32 *pData) {
    return 0;
}

s32 JASSeqParser::cmdFIRSet(JASTrack *track, u32 *pData) {
    track->setFIR((s16*)track->getSeqCtrl()->getAddr(pData[0]));
    return 0;
}

s32 JASSeqParser::cmdIIRSet(JASTrack *track, u32 *pData) {
    s16 iir[4];
    for (int i = 0; i < 4; i++) {
        iir[i] = pData[i];
    }
    track->setIIR(iir);
    return 0;
}

s32 JASSeqParser::cmdIIRCutOff(JASTrack *track, u32 *pData) {
    u32 cutOff = pData[0];
    if (cutOff < 128) {
        track->setIIR(JASCalc::CUTOFF_TO_IIR_TABLE[cutOff]);
    } 
    else {
#line 1068
        JUT_WARNING_F2("%s", "Too large cutoff");
    }
    return 0;
}

s32 JASSeqParser::cmdBankPrg(JASTrack *track, u32 *pData) {
    u16 num = JSULoHalf(pData[0]);
    track->setBankNumber(JSUHiByte(num));
    track->setProgNumber(JSULoByte(num));
    return 0;
}

s32 JASSeqParser::cmdBank(JASTrack *track, u32 *pData) {
    track->setBankNumber(pData[0] & 0xff);
    return 0;
}

s32 JASSeqParser::cmdPrg(JASTrack *track, u32 *pData) {
    track->setProgNumber(pData[0] & 0xff);
    return 0;
}

s32 JASSeqParser::cmdParamI(JASTrack *track, u32 *pData) {
    track->setParam(pData[0], s16(pData[1]) / 32767.0f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamII(JASTrack *track, u32 *pData) {
    track->setParam(pData[0], s16(pData[1]) / 32767.0f, pData[2]);
    return 0;
}

s32 JASSeqParser::cmdParamE(JASTrack *track, u32 *pData) {
    s32 *r31 = (s32*)pData + 1;
    s16 r30 = *r31 << 8;
    if ((*r31 & 0x80) == 0) {
        r30 = r30 | s16(*r31 << 1);
    }
    track->setParam(pData[0], r30 / 32767.0f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamEI(JASTrack *track, u32 *pData) {
    s32 *r31 = (s32*)pData + 1;
    s16 r30 = *r31 << 8;
    if ((*r31 & 0x80) == 0) {
        r30 = r30 | s16(*r31 << 1);
    }
    track->setParam(pData[0], r30 / 32767.0f, pData[2]);
    return 0;
}

s32 JASSeqParser::cmdReg(JASTrack *track, u32 *pData) {
    u32 reg = pData[1];
    u32 res = pData[2];
    u32 tmp;
    switch (pData[0]) {
    case 0:
        break;
    case 1:
        res += readReg(track, reg);
        break;
    case 2:
        res = readReg(track, reg) - res;
        break;
    case 3:
        res = readReg(track, reg) - res;
        reg = 3;
        break;
    case 4:
        res *= readReg(track, reg);
        reg = 0x21;
        break;
    case 5:
        res &= readReg(track, reg);
        break;
    case 6:
        res |= readReg(track, reg);
        break;
    case 7:
        res ^= readReg(track, reg);
        break;
    case 8:
        static JMARandFast oRandom(0);
        tmp = oRandom.get_bit32() >> 9;
        res = tmp % res;
        break;
    case 9:
        res = readReg(track, reg) << res;
        break;
    case 10:
        res = readReg(track, reg) >> res;
        break;
    default:
#line 1182
        JUT_WARNING_F2("%s", "cmdReg Not Supported RegCmdType");
        return 0;
    }
    writeReg(track, reg, res);
    return 0;
}

s32 JASSeqParser::cmdRegLoad(JASTrack *track, u32 *pData) {
    writeReg(track, pData[0], pData[1]);
    return 0;
}

s32 JASSeqParser::cmdRegUni(JASTrack *track, u32 *pData) {
    u32 reg = pData[1];
    s32 res = 0;
    switch (pData[0]) {
    case 11:
        res = -readReg(track, reg);
        break;
    default:
#line 1208
        JUT_WARNING_F2("%s", "cmdRegUni Not Supported RegCmdType");
        return 0;
    }
    writeReg(track, reg, res);
    return 0;
}

s32 JASSeqParser::cmdRegTblLoad(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    u32 reg = pData[1];
    u32 tblAddr = pData[2];
    u32 offset = pData[3];
    u32 res;
    switch (pData[0]) {
    case 12:
        res = seqCtrl->getByte(tblAddr + offset);
        break;
    case 13:
        offset *= 2;
        res = seqCtrl->get16(tblAddr + offset);
        break;
    case 14:
        offset += offset * 2;
        res = seqCtrl->get24(tblAddr + offset);
        break;
    case 15:
        offset *= 4;
        res = seqCtrl->get32(tblAddr + offset);
        break;
    case 16:
        res = seqCtrl->get32(tblAddr + offset);
        break;
    default:
#line 1246
        JUT_WARNING_F2("%s", "cmdRegTblLoad Not Supported RegCmdType");
        return 0;
    }
    writeReg(track, reg, res);
    return 0;
}

s32 JASSeqParser::cmdDump(JASTrack *track, u32 *pData) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    JASReport("--------------- JASTrack (%8x) dump ----------------", track);
    JASReport(" Base: 0x%08x Cur: 0x%08x(0x%06x)", seqCtrl->getBase(), seqCtrl->getCur(), (u8*)seqCtrl->getCur() - (u8*)seqCtrl->getBase());
    JASReport("");
    JASReport(" REG_A: 0x%04x REG_B: 0x%04x REG_S: 0x%04x", readReg(track, 0), readReg(track, 1), readReg(track, 2));
    JASReport(" REG_X: 0x%04x REG_Y: 0x%04x REG_F: 0x%04x", readReg(track, 4), readReg(track, 5), readReg(track, 3));
    JASReport("");
    JASReport(" PORT 0-3: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x0), track->getPort(0x1), track->getPort(0x2), track->getPort(0x3));
    JASReport(" PORT 4-7: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x4), track->getPort(0x5), track->getPort(0x6), track->getPort(0x7));
    JASReport(" PORT 8-B: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x8), track->getPort(0x9), track->getPort(0xa), track->getPort(0xb));
    JASReport(" PORT C-F: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0xc), track->getPort(0xd), track->getPort(0xe), track->getPort(0xf));
    JASReport("");
    return 0;
}

s32 JASSeqParser::cmdPrintf(JASTrack *track, u32 *pData) {
    u8 dataType[4];
    u32 stack_10[4];
    char buffer[128];
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    u32 n = 0;
    
    for (u32 i = 0; i < 128; i++) {
        buffer[i] = seqCtrl->readByte();
        if (buffer[i] == 0) {
            break;
        }
        if (buffer[i] == '\\') {
            buffer[i] = seqCtrl->readByte();
            if (buffer[i] == 0) {
                break;
            }
            switch (buffer[i]) {
            case 'n':
                buffer[i] = '\r';
                break;
            }            
        } 
        else if (buffer[i] == '%') {
            i++;
            buffer[i] = seqCtrl->readByte();
            if (buffer[i] == 0) {
                break;
            }
            switch (buffer[i]) {
            case 'd':
                dataType[n] = 0;
                break;
            case 'x':
                dataType[n] = 1;
                break;
            case 's':
                dataType[n] = 2;
                break;
            case 'r':
                dataType[n] = 3;
                buffer[i] = 'd';
                break;
            case 'R':
                dataType[n] = 4;
                buffer[i] = 'x';
                break;
            default:
                break;
            }
            n++;
        }
    }
    for (u32 i = 0; i < n; i++) {
        stack_10[i] = seqCtrl->readByte();
        switch (dataType[i]) {
        case 2:
            stack_10[i] = (u32)seqCtrl->getAddr(stack_10[i]);
            break;
        case 3:
        case 4:
            stack_10[i] = readReg(track, stack_10[i] & 0xff);
            break;
        default:
            break;
        }
    }
    JASReport(buffer, stack_10[0], stack_10[1], stack_10[2], stack_10[3]);
    return 0;
}

s32 JASSeqParser::execNoteOnGate(JASTrack *track, u32 pData, u32 p2, u32 p3,
                                 u32 p4) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    // likely fake match, this may use some actual union defined somewhere else
    union {
        u8 val;
        struct {
            u8 bit0 : 1;
            u8 bit1 : 1;
        } bits;
    } tmp;
    tmp.val = p4;
    int r31 = 0;
    if (tmp.bits.bit1) {
        r31 |= 2;
    }
    if (tmp.bits.bit0) {
        r31 |= 1;
    }
    if (p3 == 0) {
        r31 |= 4;
    }
    s32 result = track->gateOn(pData, p2, p3, r31);
    if (p3) {
        seqCtrl->wait(p3);
    } 
    else {
        seqCtrl->waitNoteFinish();
    }
    return result;
}

s32 JASSeqParser::execNoteOnMidi(JASTrack *track, u32 pData, u32 p2, u32 p3) {
     return track->noteOn(pData, p2, p3);
}

s32 JASSeqParser::execNoteOff(JASTrack *track, u32 pData) {
    return track->noteOff(pData, 0);
}

s32 JASSeqParser::execCommand(JASTrack *track, s32 (JASSeqParser::*pData)(JASTrack*, u32*),
                                   u32 p2, u32 *p3) {
    return (this->*pData)(track, p3);
}

s32 JASSeqParser::parseNoteOff(JASTrack *track, u8 note_id) {
    if (note_id & 7) {
        execNoteOff(track, note_id & 7);
    } 
    else {
#line 1424
        JUT_WARNING_F2("%s", "noteoff for note_id == 0");
    }
    return 0;
}

s32 JASSeqParser::parseNoteOn(JASTrack *track, u8 p2) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    u8 b = seqCtrl->readByte();
    u8 r28 = b;
    r28 &= 7;
    u8 r27 = seqCtrl->readByte();
    if (r28 == 0) {
        u32 midiVal = seqCtrl->readMidiValue();
        execNoteOnGate(track, p2, r27, midiVal, b);
    } 
    else {
        execNoteOnMidi(track, r28, p2, r27);
    }
    return 0;
}

// Size mismatch 
s32 JASSeqParser::parseCommand(JASTrack *track, u8 cmd, u16 p2) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    CmdInfo *cmdInfo = NULL;
    if (cmd != 0xb0) {
#line 1468
        JUT_ASSERT(cmd >= 0xa0);
        cmdInfo = &sCmdInfo[cmd - 0xa0];
    } 
    else {        
        cmdInfo = &sExtCmdInfo[seqCtrl->readByte()];
    }
    u16 r28 = cmdInfo->_e;
    r28 |= p2;
    u32 data[8];
    for (int i = 0; i < cmdInfo->branchCondition; i++) {
        u32 seqData = 0;
        switch (r28 & 3) {
        case 0:
            seqData = seqCtrl->readByte();
            break;
        case 1:
            seqData = seqCtrl->read16();
            break;
        case 2:
            seqData = seqCtrl->read24();
            break;
        case 3:
            seqData = readReg(track, seqCtrl->readByte());
            break;
        }
        data[i] = seqData;
        r28 >>= 2;
    }
    s32 (JASSeqParser::*ptr)(JASTrack*, u32*) = cmdInfo->func;
    if (!ptr) {
        return 0;
    }
    return execCommand(track, ptr, cmdInfo->branchCondition, data);
}

s32 JASSeqParser::parseRegCommand(JASTrack *track, int n) {
    JASSeqCtrl *seqCtrl = track->getSeqCtrl();
    u8 r30 = seqCtrl->readByte();
    u16 r29 = 0;
    u16 r28 = 3;
    for (int i = 0; i < n; i++) {
        if (r30 & 0x80) {
            r29 |= r28;
        }
        r30 <<= 1;
        r28 <<= 2;
    }
    return parseCommand(track, seqCtrl->readByte(), r29);
}

s32 JASSeqParser::parse(JASTrack *track) {
    u8 r31 = track->getSeqCtrl()->readByte();
    s32 ret = 0;
    if ((r31 & 0x80) == 0) {
        ret = parseNoteOn(track, r31);
    } 
    else {
        switch (r31 & 0xf0) {
        case 0x80:
            ret = parseNoteOff(track, r31 & 0xf);
            break;
        case 0x90:
            ret = parseRegCommand(track, (r31 & 7) + 1);
            break;
        default:
            ret = parseCommand(track, r31, 0);
            break;
        }
    }
    return ret;
}
