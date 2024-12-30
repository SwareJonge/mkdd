#ifndef JAUDIO_JASSEQPARSER_H
#define JAUDIO_JASSEQPARSER_H

#include "JSystem/JAudio/System/JASTrack.h"

class JASSeqParser { // Autogenerated
public:
    enum BranchCondition {};

    struct CmdInfo {
        s32 (JASSeqParser::*func)(JASTrack*track, u32*);
        u16 branchCondition;
        u16 _e;
    };

    virtual ~JASSeqParser() {} // 08
    virtual s32 parse(JASTrack *track); // 0C, 0x8009f000
    virtual s32 execNoteOnMidi(JASTrack *track, u32, u32, u32); // 10, 0x8009ef74
    virtual s32 execNoteOnGate(JASTrack *track, u32, u32, u32, u32); // 14, 0x8009eecc
    virtual s32 execNoteOff(JASTrack *track, u32); // 18, 0x8009efa4
    virtual s32 execCommand(JASTrack *track, s32 (JASSeqParser::*)(JASTrack *track, u32 *), u32, u32 *); // 1C, 0x8009efd4
    
    void writeReg(JASTrack *track, u32, u32); // 0x8009d31c
    u32 readReg(JASTrack *track, u32) const; // 0x8009d548
    s32 cmdOpenTrack(JASTrack *track, u32 *); // 0x8009d78c
    s32 cmdCloseTrack(JASTrack *track, u32 *); // 0x8009d820
    s32 cmdCall(JASTrack *track, u32 *); // 0x8009d84c
    s32 cmdCallF(JASTrack *track, u32 *); // 0x8009d878
    s32 cmdRet(JASTrack *track, u32 *); // 0x8009d9a8
    s32 cmdRetF(JASTrack *track, u32 *); // 0x8009da04
    s32 cmdJmp(JASTrack *track, u32 *); // 0x8009db58
    s32 cmdJmpF(JASTrack *track, u32 *); // 0x8009db70
    s32 cmdJmpTable(JASTrack *track, u32 *); // 0x8009dca4
    s32 cmdCallTable(JASTrack *track, u32 *); // 0x8009dcd8
    s32 cmdLoopS(JASTrack *track, u32 *); // 0x8009dd24
    s32 cmdLoopE(JASTrack *track, u32 *); // 0x8009dd50
    s32 cmdNote(JASTrack *track, u32 *); // 0x8009dd78
    s32 cmdNoteOn(JASTrack *track, u32 *); // 0x8009ddbc
    s32 cmdNoteOff(JASTrack *track, u32 *); // 0x8009ddfc
    s32 cmdReadPort(JASTrack *track, u32 *); // 0x8009de30
    s32 cmdWritePort(JASTrack *track, u32 *); // 0x8009de98
    s32 cmdParentWritePort(JASTrack *track, u32 *); // 0x8009decc
    s32 cmdChildWritePort(JASTrack *track, u32 *); // 0x8009df30
    s32 cmdParentReadPort(JASTrack *track, u32 *); // 0x8009dff0
    s32 cmdChildReadPort(JASTrack *track, u32 *); // 0x8009e088
    s32 cmdCheckPortImport(JASTrack *track, u32 *); // 0x8009e15c
    s32 cmdCheckPortExport(JASTrack *track, u32 *); // 0x8009e1a8
    s32 cmdWait(JASTrack *track, u32 *); // 0x8009e1f4
    s32 cmdWaitByte(JASTrack *track, u32 *); // 0x8009e22c
    s32 cmdSetLastNote(JASTrack *track, u32 *); // 0x8009e23c
    s32 cmdEnvSet(JASTrack *track, u32 *); // 0x8009e29c
    s32 cmdSimpleADSR(JASTrack *track, u32 *); // 0x8009e2d4
    s32 cmdBusConnect(JASTrack *track, u32 *); // 0x8009e328
    s32 cmdSetIntTable(JASTrack *track, u32 *); // 0x8009e358
    s32 cmdSetInterrupt(JASTrack *track, u32 *); // 0x8009e368
    s32 cmdDisInterrupt(JASTrack *track, u32 *); // 0x8009e394
    s32 cmdClrI(JASTrack *track, u32 *); // 0x8009e3c0
    s32 cmdRetI(JASTrack *track, u32 *); // 0x8009e3f0
    s32 cmdIntTimer(JASTrack *track, u32 *); // 0x8009e42c
    s32 cmdSyncCPU(JASTrack *track, u32 *); // 0x8009e448
    s32 cmdTempo(JASTrack *track, u32 *); // 0x8009e4b0
    s32 cmdFinish(JASTrack *track, u32 *); // 0x8009e4e0
    s32 cmdNop(JASTrack *track, u32 *); // 0x8009e4e8
    s32 cmdFIRSet(JASTrack *track, u32 *); // 0x8009e4f0
    s32 cmdIIRSet(JASTrack *track, u32 *); // 0x8009e524
    s32 cmdIIRCutOff(JASTrack *track, u32 *); // 0x8009e570
    s32 cmdBankPrg(JASTrack *track, u32 *); // 0x8009e5dc
    s32 cmdBank(JASTrack *track, u32 *); // 0x8009e5f4
    s32 cmdPrg(JASTrack *track, u32 *); // 0x8009e604
    s32 cmdParamI(JASTrack *track, u32 *); // 0x8009e614
    s32 cmdParamII(JASTrack *track, u32 *); // 0x8009e670
    s32 cmdParamE(JASTrack *track, u32 *); // 0x8009e6cc
    s32 cmdParamEI(JASTrack *track, u32 *); // 0x8009e748
    s32 cmdReg(JASTrack *track, u32 *); // 0x8009e7c4
    s32 cmdRegLoad(JASTrack *track, u32 *); // 0x8009e960
    s32 cmdRegUni(JASTrack *track, u32 *); // 0x8009e990
    s32 cmdRegTblLoad(JASTrack *track, u32 *); // 0x8009ea30
    s32 cmdDump(JASTrack *track, u32 *); // 0x8009eb28
    s32 cmdPrintf(JASTrack *track, u32 *); // 0x8009ecb8

    // Inline/Unused
    bool conditionCheck(JASTrack *track, BranchCondition);
    s32 parseNoteOff(JASTrack *track, unsigned char);
    s32 parseNoteOn(JASTrack *track, unsigned char);
    s32 parseCommand(JASTrack *track, unsigned char, unsigned short);
    s32 parseRegCommand(JASTrack *track, int);

    static void registerSeqCallback(u16 (*param_0)(JASTrack*, u16)) { sCallBackFunc = param_0; }

    static CmdInfo sCmdInfo[96]; // 0x8038bd94
    static CmdInfo sExtCmdInfo[255]; // 0x8038c3a0
    static u16 (*sCallBackFunc)(JASTrack*, u16); // 0x80415ae0

}; // namespace JASSeqParser
#endif // JAUDIO_JASSEQPARSER_H