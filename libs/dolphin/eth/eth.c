#include <string.h>

#include <dolphin/eth/eth.h>
#include <dolphin/exi.h>
#include <dolphin/hw_regs.h>
#include <dolphin/os.h>

#include "types.h"

#ifdef DEBUG
const char* __ETHVersion = "<< Dolphin SDK - ETH\tdebug build: Aug  5 2003 17:18:42 (0x2301) >>";
#else
const char* __ETHVersion = "<< Dolphin SDK - ETH\trelease build: Aug  5 2003 17:18:42 (0x2301) >>";
#endif

static u8 broadcastheader[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static u8 private_macaddr[6];
static u8 __revid = 0;
static u16 __devid = 0xD107;
static u8 __acstart = 0x4E;
static OSThreadQueue threadQ;
static BOOL lockUpdate = FALSE;
static BOOL __sendbusy = FALSE;
static BOOL sendUpdate = FALSE;
static ETHStat ethstat;
static u16* protoarray = NULL;
static s32 protonum = 0;
static void* (*recvcallback0)(u16, s32, u8) = NULL;
static void (*recvcallback1)(u8*, s32) = NULL;
static BOOL recvcallbackUpdate = FALSE;
static void (*__sendcallback)(u8) = NULL;
static BOOL __recvpriorityflag = FALSE;
static u8* __sendadr = NULL;
static s32 __sendlen = 0;
static s32 __senddmalen = 0;
static s32 __sendimmlen = 0;
static u8 tmppool0[1536] ATTRIBUTE_ALIGN(32);
static u8 tmppool1[1536] ATTRIBUTE_ALIGN(32);
static u32 recvlen0 = 0;
static u32 recvlen1 = 0;

typedef struct Descriptor {
    u8 nextpacketL;
    u8 packetlengthL : 4;
    u8 nextpacketH : 4;
    u8 packetLengthH;
    u8 status;
} Descriptor;

typedef struct Descriptor2 {
    u16 nextpacket;
    u16 packetlength;
    u8 status;
} Descriptor2;

static Descriptor2 d2;

static u8* recvaddr = NULL;
static u16 rwp = 0;
static u16 rrp = 0;
static u8 mcastmap[8];
static BOOL mcastUpdate = FALSE;


static BOOL lock(void);
static void unlock(void);

static void recvsub0(void);
static void recvsub1(void);

static void sendsub0(void);
static void sendsub1(s32 chan, OSContext* context);

static void readbuffer0(u16 cmd);
static void readbuffer1(s32 chan, OSContext* context);
static void readbuffersub(s32 chan, OSContext* context);

static void exiinthandler(s32 chan, OSContext* context);

static inline u16 swap16(u16 val) {
    u16 ret = __lhbrx(&val, 0);
    return ret;
}

static inline u8 higher(u16 val) {
    return (val >> 8) & 0xff;
}

static inline u8 lower(u16 val) {
    return val & 0xff;
}

static void waitexilock(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    while (TRUE) {
        if (lock() != FALSE) {
            OSRestoreInterrupts(enabled);
            return;
        }

        lockUpdate = TRUE;
        OSSleepThread(&threadQ);
        OSRestoreInterrupts(enabled);
    }
}

static void waitmicro(s32 usec) {
    OSTick start;
    OSTick interval;

    start = OSGetTick();
    interval = OSMicrosecondsToTicks(usec);

    while (TRUE) {
        if (OSGetTick() - start >= interval) {
            break;
        }
    }
}

static u8 readEXIcmd(u8 cmd) {
    u16 ethcmd;
    u8 rdata;

    ethcmd = 0x0000 | (cmd << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImm(0, &rdata, sizeof(rdata), EXI_READ, NULL);
    EXISync(0);
    EXIDeselect(0);
    return rdata;
}

static u8 readEXIcmdWait200Micro(u8 cmd) {
    u16 ethcmd;
    u8 rdata;

    ethcmd = 0x0000 | (cmd << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImm(0, &rdata, sizeof(rdata), EXI_READ, NULL);
    EXISync(0);
    waitmicro(200);
    EXIDeselect(0);
    return rdata;
}

static void writeEXIcmd(u8 cmd, u8 imr) {
    BOOL ret;
    u16 ethcmd;

    ethcmd = 0x4000 | (cmd << 8);
    ret = EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImm(0, &imr, sizeof(imr), EXI_WRITE, NULL);
    EXISync(0);
    EXIDeselect(0);
}

static void readEXIcmdLong(u8 cmd, void* address, s32 length) {
    u16 ethcmd;

    ethcmd = cmd << 8;
    EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImmEx(0, address, length, EXI_READ);
    EXIDeselect(0);
}

static void writeEXIcmdLong(u8 cmd, void* address, s32 length) {
    BOOL ret;
    u16 ethcmd;

    ethcmd = 0x4000 | (cmd << 8);
    ret = EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImmEx(0, address, length, EXI_WRITE);
    EXIDeselect(0);
}

static void readcmdLong(u16 cmd, void*  addr, s32 length) {
    u32 ethcmd;

    ethcmd = 0x80000000 | (cmd << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImmEx(0, addr, length, EXI_READ);
    EXIDeselect(0);
}

static void writecmdLong(u16 cmd, void* addr, s32 length) {
    u32 ethcmd = 0xC0000000 | (cmd << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &ethcmd, sizeof(ethcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIImmEx(0, addr, length, EXI_WRITE);
    EXIDeselect(0);
}

static u8 readcmd(u16 cmd) {
    u8 val;

    readcmdLong(cmd, &val, sizeof(val));
    return val;
}

static void writecmd(u16 cmd, u8 val) {
    writecmdLong(cmd, &val, sizeof(val));
}

static void StaOutput(unsigned int bit) {
    writecmd(0x0060, bit * 2);
    writecmd(0x0060, 8 + bit * 2);
    writecmd(0x0060, 8 + bit * 2);
    writecmd(0x0060, bit * 2);
}

static u8 StaInput(void) {
    unsigned char bit;

    writecmd(0x0060, 0x0C);
    bit = readcmd(0x0060);
    bit &= 1;
    writecmd(0x0060, 0x04);
    writecmd(0x0060, 0x04);
    return bit;
}

static u16 PhyRead(unsigned int regNo) {
    int i;
    u16 value;

    /* Clear command */
    for (i = 0; i < 32; i++) {
        StaOutput(1);
    }

    /* Enter read mode */
    StaOutput(0);
    StaOutput(1);
    StaOutput(1);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);

    /* Set register */
    StaOutput((regNo >> 4) & 1);
    StaOutput((regNo >> 3) & 1);
    StaOutput((regNo >> 2) & 1);
    StaOutput((regNo >> 1) & 1);
    StaOutput((regNo >> 0) & 1);
    writecmd(0x0060, 4);
    value = StaInput();
    value = StaInput();
    
    /* Read value */
    value = 0;
    for (i = 0; i < 16; i++) {
        value = value + (StaInput() << (15 - i));
    }

    /* Clear command */
    for (i = 0; i < 32; i++) {
        StaOutput(1);
    }

    return value;
}

static void PhyWrite(unsigned int regNo, u16 value) {
    int i;

    /* Clear command */
    for (i = 0; i < 32; i++) {
        StaOutput(1);
    }

    /* Enter write mode */
    StaOutput(0);
    StaOutput(1);
    StaOutput(0);
    StaOutput(1);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    StaOutput(0);
    
    /* Set register */
    StaOutput((regNo >> 4) & 1);
    StaOutput((regNo >> 3) & 1);
    StaOutput((regNo >> 2) & 1);
    StaOutput((regNo >> 1) & 1);
    StaOutput((regNo >> 0) & 1);

    StaOutput(1);
    StaOutput(0);

    /* Write value */
    for (i = 0; i < 16; i++) {
        StaOutput((u8)((value >> (15 - i)) & 1));
    }

    /* Clear command */
    for (i = 0; i < 32; i++) {
        StaOutput(1);
    }
}

static void reset(void) {
    writecmd(0x0060, 0);
    waitmicro(10000);
    readEXIcmdWait200Micro(0x0F);
    waitmicro(10000);
    writecmd(0x0000, 1);
    writecmd(0x0000, 0);
}

static void recvinit(void) {
    u8 ncrb;
    u16 swapped;

    swapped = higher(0x0001) | (lower(0x0001) << 8);
    writecmdLong(0x000A, &swapped, sizeof(swapped));
    writecmdLong(0x0016, &swapped, sizeof(swapped));
    writecmdLong(0x0018, &swapped, sizeof(swapped));

    swapped = higher(0x000F) | (lower(0x000F) << 8);
    writecmdLong(0x001A, &swapped, sizeof(swapped));

    ncrb = readcmd(0x0001);
    ncrb &= ~0b00000011; // ~3
    ncrb |= 0b00010000; // | 0x10
    writecmd(0x0001, ncrb);
    writecmd(0x0000, 0x08);
    writecmd(0x0032, 0x08);
}

static void getdescriptor(u16 address) {
    Descriptor d;

    readcmdLong(address, &d, sizeof(d));
    d2.nextpacket = (d.nextpacketH << 8) | d.nextpacketL;
    d2.packetlength = (d.packetLengthH << 4) | d.packetlengthL;
    d2.status = d.status;
}

static u16 readcmd16(u8 cmd) {
    u16 tmp16;

    readcmdLong(cmd, &tmp16, sizeof(tmp16));
    
    return swap16(tmp16);
}

static void writecmd16(u8 cmd, u16 val) {
    u16 tmp16;

    tmp16 = swap16(val);
    writecmdLong(cmd, &tmp16, sizeof(tmp16));
}

static void recvsub0(void) {
    rwp = readcmd16(0x16);
    rrp = readcmd16(0x18);
    recvsub1();
}

static void recvsub1(void) {
    u16 packettype;
    s32 i;
    u8 lrps;
    u16 address;
    
    recvlen0 = recvlen1 = 0;
    do {
        while (rrp != rwp) {
            address = (rrp & 0xFF) << 8;
            getdescriptor(address);
            readcmdLong(address + 0x10U, &packettype, sizeof(packettype));
    
            if (protonum > 0) {
                for (i = 0; i < protonum; i++) {
                    if (packettype == protoarray[i]) {
                        break;
                    }
                }
                
                if (i == protonum) {
                    goto update_pointers;
                }
            }

            if (recvcallback0) {
                lrps = d2.status;
                recvaddr = (*recvcallback0)(packettype, d2.packetlength, lrps);

                if (recvaddr != NULL) {
                    if (address + 4U + d2.packetlength > 0x1000U) {
                        recvlen1 = address - 0x0FFCU + d2.packetlength;
                        recvlen0 = d2.packetlength - recvlen1;
                    } else {
                        recvlen0 = d2.packetlength;
                        recvlen1 = 0;
                    }

                    readbuffer0(address + 4U);
                    return;
                }
            }

update_pointers:
            rrp = d2.nextpacket;
            rwp = readcmd16(0x16);
        }
        
        writecmd16(0x18, rwp);
        writecmd(0x0009, 0x02);
        rwp = readcmd16(0x16);
        rrp = readcmd16(0x18);
    } while (rwp != rrp);

    writeEXIcmd(0x02, 0xF8);
    unlock();
}

static void readbuffer0(u16 cmd) {
    u32 etcmd;
    u32 length;

    length = OSRoundUp32B(recvlen0);
    etcmd = 0x80000000 | (cmd << 8);

    EXISelect(0, 2, 5);
    EXIImm(0, &etcmd, sizeof(etcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIDma(0, tmppool0, length, EXI_READ, recvlen1 != 0 ? &readbuffersub : &readbuffer1);
}

static void readbuffersub(s32 chan, OSContext* context) {
    u32 etcmd;
    u32 length;

    length = OSRoundUp32B(recvlen1);
    EXIDeselect(0);
    
    etcmd = 0x80000000 | (0x0100 << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &etcmd, sizeof(etcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIDma(0, tmppool1, length, EXI_READ, readbuffer1);
    __ETHInterruptTime = OSGetTime() - __OSLastInterruptTime;
}

static void readbuffer1(s32 chan, OSContext* context) {
    EXIDeselect(0);
    
    DCInvalidateRange(tmppool0, OSRoundUp32B(recvlen0));
    if (recvlen1 != 0) {
        DCInvalidateRange(tmppool1, OSRoundUp32B(recvlen1));
    }

    if (recvcallback1) {
        memcpy(recvaddr, tmppool0, recvlen0);
        if (recvlen1 != 0) {
            memcpy(recvaddr + recvlen0, tmppool1, recvlen1);
        }
        __ETHFilter(recvaddr, d2.packetlength);
        (*recvcallback1)(recvaddr, d2.packetlength);
    }

    rrp = d2.nextpacket;
    rwp = readcmd16(0x16);
    recvsub1();
    __ETHInterruptTime = OSGetTime() - __OSLastInterruptTime;
}

#include <dolphin/eth/hashfunc.c>

static void patchthru(void) {
    u32 rnda;
    u32 fa;

    writeEXIcmd(0x05, __acstart);
    readEXIcmdLong(0x08, &rnda, sizeof(rnda));
    fa = hashfunc(rnda);
    writeEXIcmdLong(0x09, &fa, sizeof(fa));
}

static void exiinthandler(s32 chan, OSContext* context) {
    u8 ir;
    u8 exiisr;
    BOOL ret;
    BOOL sendcbflag;
    u8 ltps;
    u8 lrps;
    u8 result;
    void (*callback)(u8);

    sendcbflag = FALSE;
    ret = EXILock(0, 2, &exiinthandler);
    if (ret != FALSE) {
        exiisr = readEXIcmd(0x03);
        writeEXIcmd(0x02, 0x00);

        switch (__cntlzw(exiisr)) {
            case 0x18: {
                writeEXIcmd(0x03, 0x80);
                break;
            }

            case 0x1A: {
                writeEXIcmd(0x03, 0x20);
                writeEXIcmd(0x02, 0xF8);
                unlock();
                #ifdef DEBUG
                OSPanic(__FILE__, 765, "exi cmderr\n");
                #endif
                return;
            }

            case 0x19: {
                writeEXIcmd(0x03, 0x40);
                writeEXIcmd(0x02, 0xF8);
                unlock();
                return;
            }

            case 0x1B: {
                patchthru();
                writeEXIcmd(0x03, 0x10);
                writeEXIcmd(0x02, 0xF8);
                unlock();
                return;
            }

            case 0x1C: {
                result = readEXIcmd(0x0B);
                writeEXIcmd(0x03, 0x08);
                writeEXIcmd(0x02, 0xF8);
                unlock();

                if (result != 1) {
                    #ifdef DEBUG
                    OSPanic(__FILE__, 785, "ETHLIB: hash func error\n");
                    #endif
                }

                return;
            }
        }

        ir = readcmd(0x0009);
        if (__recvpriorityflag == TRUE && (ir & 2) != 0) {
            ethstat.rcvcount++;
            recvsub0();
            __ETHInterruptTime = OSGetTime() - __OSLastInterruptTime;
        } else {
            switch (__cntlzw(ir)) {
                case 0x18: {
                    ethstat.rbf++;
                    writecmd(0x0009, 0x80);
                    break;
                }

                case 0x19: {
                    writecmd(0x0009, 0x40);
                    break;
                }

                case 0x1A: {
                    ethstat.fifoe++;
                    writecmd(0x0009, 0x20);
                    break;
                }

                case 0x1B: {
                    ethstat.te++;
                    //ASSERTLINE(816, __sendbusy);
                    __sendbusy = FALSE;
                    ltps = readcmd(0x0004);

                    if ((ltps & 0x10) != 0) {
                        ethstat.tx_crs++;
                    }

                    if ((ltps & 0x20) != 0) {
                        ethstat.tx_uf++;
                    }

                    if ((ltps & 0x40) != 0) {
                        ethstat.tx_owc++;
                    }

                    sendcbflag = TRUE;
                    writecmd(0x0009, 0x10);
                    break;
                }
                
                case 0x1C: {
                    ethstat.re++;

                    lrps = readcmd(0x0005);
                    if ((lrps & 1) != 0) {
                        ethstat.rx_bf++;
                    }

                    if ((lrps & 2) != 0) {
                        ethstat.rx_crc++;
                    }

                    if ((lrps & 4) != 0) {
                        ethstat.rx_fae++;
                    }

                    if ((lrps & 8) != 0) {
                        ethstat.rx_fo++;
                    }

                    if ((lrps & 0x10) != 0) {
                        ethstat.rx_rw++;
                    }

                    if ((lrps & 0x40) != 0) {
                        ethstat.rx_rf++;
                    }

                    writecmd(0x0009, 0x08);
                    break;
                }

                case 0x1D: {
                    //ASSERTLINE(868, __sendbusy);
                    ethstat.sendcount++;
                    __sendbusy = FALSE;
                    sendcbflag = TRUE;
                    ltps = readcmd(0x0004);
                    writecmd(0x0009, 0x04);
                    break;
                }

                case 0x1E: {
                    ethstat.rcvcount++;
                    recvsub0();
                    return;
                }

                case 0x1F: {
                    ethstat.cntof++;
                    writecmd(0x0009, 0x01);
                    break;
                }
            }

            writeEXIcmd(0x02, 0xF8);
            unlock();
            
            if (sendcbflag) {
                callback = __sendcallback;
                __sendcallback = NULL;
                __ETHPostSend(ltps, callback, __sendadr);
            }

            __ETHInterruptTime = OSGetTime() - __OSLastInterruptTime;
        }
    }
}

s32 ETHInit(s32 mode) {
    static BOOL initialized = FALSE;
    u32 cid;
    u8 fcsr;
    u8 phy0r;
    
    if (initialized == FALSE) {
        initialized = TRUE;
        OSRegisterVersion(__ETHVersion);
    }

    OSInitThreadQueue(&threadQ);
    __SIRegs[15] = __SIRegs[15] & ~0x80000000; // turn on 32MHz EXI clock setting allowance
    waitexilock();
    __sendbusy = FALSE;
    memset(&ethstat, 0, sizeof(ethstat));
    protoarray = NULL;
    protonum = 0;
    recvcallback0 = NULL;
    recvcallback1 = NULL;

    cid = 0;
    EXIGetID(0, 2, &cid);
    if (cid != EXI_ETHER) {
        unlock();
        OSReport("cid = %08x\n", cid);
        return -1;
    } else {
        reset();
        readcmdLong(0x0020, private_macaddr, sizeof(private_macaddr));
        __revid = readEXIcmd(0x01);
        writeEXIcmdLong(0x04, &__devid, sizeof(__devid));
        writeEXIcmd(0x05, __acstart);
    
        fcsr = readcmd(0x005B);
        fcsr &= ~0x80;
        writecmd(0x005B, fcsr);
        writecmd(0x005E, 0x01);
    
        phy0r = readcmd(0x005C);
        phy0r |= 0x04;
        writecmd(0x005C, phy0r);
        
        writecmd(0x0001, 0x00);
        writecmd(0x0003, 0x2E);
        writecmd(0x0050, 0x80);
        writecmd(0x0008, 0x00);
        writeEXIcmd(0x02, 0xF8);
        EXISetExiCallback(2, (EXICallback)&exiinthandler);
        unlock();
        
        return ETH_OK;
    }
}

void ETHSendAsync(void* addr, s32 length, void (*callback2)(u8)) {
    BOOL disabled;
    
    //ASSERTLINE(985, !__sendbusy);
    //ASSERTLINE(986, ((u32) addr % 32) == 0);
    DCStoreRange(addr, length);

    disabled = OSDisableInterrupts();
    __sendbusy = TRUE;
    __sendadr = addr;
    __sendlen = length < 60 ? 60 : length;
    __sendcallback = callback2;
    sendUpdate = TRUE;

    if (lock()) {
        unlock();
    }

    OSRestoreInterrupts(disabled);
}

static void sendsub0(void) {
    u32 etcmd;
    
    __senddmalen = OSRoundDown32B(__sendlen);
    __sendimmlen = __sendlen - __senddmalen;

    etcmd = 0xC0000000 | (0x0048 << 8);
    EXISelect(0, 2, 5);
    EXIImm(0, &etcmd, sizeof(etcmd), EXI_WRITE, NULL);
    EXISync(0);
    EXIDma(0, __sendadr, __senddmalen, EXI_WRITE, &sendsub1);
}

static void sendsub1(s32 chan, OSContext* context) {
    u8 ncra;

    if (__sendimmlen != 0) {
        EXIImmEx(0, __sendadr + __senddmalen, __sendimmlen, EXI_WRITE);
    }

    EXIDeselect(0);
    ncra = readcmd(0x0000);
    //ASSERTLINE(1033, (ncra & ME_NCRA_TXFIFO) == 0);
    ncra |= ME_NCRA_TXFIFO;
    writecmd(0x0000, ncra);
    unlock();
}

void ETHGetMACAddr(u8* macaddr) {
    memcpy(macaddr, private_macaddr, sizeof(private_macaddr));
}

void ETHSetMACAddr(u8* macaddr) {
    memcpy(private_macaddr, macaddr, sizeof(private_macaddr));
    waitexilock();
    writecmdLong(0x0020, private_macaddr, sizeof(private_macaddr));
    unlock();
}

void ETHSetRecvCallback(void* (*cb0)(u16, s32, u8), void (*cb1)(u8*, s32)) {
    BOOL disabled;

    disabled = OSDisableInterrupts();
    recvcallback0 = cb0;
    recvcallback1 = cb1;
    recvcallbackUpdate = TRUE;

    if (lock()) {
        unlock();
    }

    OSRestoreInterrupts(disabled);
}

static BOOL linkState(void) {
    u8 nways;

    nways = readcmd(0x0031);
    if ((nways & 1) != 0 || (nways & 2) != 0) {
        return TRUE;
    }

    return FALSE;
}

static BOOL ETHGetLinkState(void) {
    BOOL ret;
    
    waitexilock();
    ret = linkState();
    unlock();
    return ret;
}

static BOOL linkStateAsyncMain(BOOL* linkstateptr) {
    if (EXILock(0, 2, NULL)) {
        *linkstateptr = linkState();
        unlock();
        return TRUE;
    }

    return FALSE;
}

BOOL ETHGetLinkStateAsync(BOOL* status) {
    //ASSERTLINE(1104, status);
    return linkStateAsyncMain(status);
}

s32 ETHGetNWAYMode(void) {
    u8 nways;

    waitexilock();
    nways = readcmd(0x0031);
    unlock();

    if ((nways & 0x10) != 0) {
        return 1;
    }

    if ((nways & 0x20) != 0) {
        return 2;
    }

    if ((nways & 0x40) != 0) {
        return 3;
    }

    if ((nways & 0x80) != 0) {
        return 4;
    }

    return 5;
}

void ETHSetProtoType(u16* array, s32 num) {
    BOOL flag;
    
    //ASSERTLINE(1139, (num != 0 && array != NULL) || (num == 0 && array == NULL));
    flag = OSDisableInterrupts();
    protoarray = array;
    protonum = num;
    OSRestoreInterrupts(flag);
}

void ETHGetStatus(ETHStat* stat) {
    BOOL flag;

    //ASSERTLINE(1150, stat);
    flag = OSDisableInterrupts();
    *stat = ethstat;
    OSRestoreInterrupts(flag);
}

s32 ETHGetLibraryVersion(void) {
    return 0x506;
}

s32 ETHGetBBAType(void) {
    return 2;
}

static u16 HashIndex(const u8* address) {
    u32 crc;
    u16 index;
    u32 msb;
    u8 byte;
    s32 byteLen;
    s32 bit;
    s32 shift;

    crc = 0xFFFFFFFF;
    index = 0;

    for (byteLen = 0; byteLen < 6; byteLen++, address++) {
        byte = *address;
        bit = 0;
        for (bit = 0; bit < 8; bit++, byte >>= 1) {
            msb = (crc >> 31) & 1;
            crc = crc << 1;

            if (msb ^ (byte & 1)) {
                crc ^= 0x04C11DB6;
                crc |= 1;
            }
        }
    }

    bit = 31;
    for (shift = 5; shift >= 0; shift--, bit--) {
        index |= ((crc >> bit) & 1) << shift;
    }

    return index;
}

void ETHAddMulticastAddress(const u8* macaddr) {
    BOOL disabled;
    u16 index;
    u8 marNo;
    u8 bitNo;
    u8 ncrb;

    if (memcmp(macaddr, broadcastheader, sizeof(broadcastheader)) == 0) {
        waitexilock();
        ncrb = readcmd(0x0001);
        ncrb |= 4;
        writecmd(0x0001, ncrb);
        unlock();
    } else {
        index = HashIndex(macaddr);
        marNo = index / 8;
        bitNo = index % 8;
        disabled = OSDisableInterrupts();
        mcastmap[marNo] |= 1 << bitNo;
        mcastUpdate = TRUE;

        if (lock()) {
            unlock();
        }

        OSRestoreInterrupts(disabled);
    }
}

void ETHChangeIntPriority(BOOL flag) {
    __recvpriorityflag = flag;
}

u32 ETHGetREVID(void) {
    u32 revid;

    waitexilock();
    revid = readEXIcmd(0x01);
    unlock();
    return revid;
}

void ETHClearMulticastAddresses(void) {
    BOOL disabled;

    disabled = OSDisableInterrupts();
    memset(mcastmap, 0, sizeof(mcastmap));
    mcastUpdate = TRUE;
    if (lock()) {
        unlock();
    }
    OSRestoreInterrupts(disabled);
}

static void unlockcallback(s32 chan, OSContext* context) {
    if (lock()) {
        unlock();
    }
}

static BOOL lock() {
    return EXILock(0, 2, &unlockcallback);
}

static void unlock(void) {
    BOOL disabled;

    disabled = OSDisableInterrupts();
    
    if (mcastUpdate) {
        mcastUpdate = FALSE;
        writecmdLong(0x0026, mcastmap, sizeof(mcastmap));
    }

    if (recvcallbackUpdate) {
        recvcallbackUpdate = FALSE;

        if (recvcallback0 == NULL) {
            writecmd(0x0000, 0x00);
            writecmd(0x0008, 0x14);
        } else {
            //ASSERTLINE(1308, recvcallback0 != NULL && recvcallback1 != NULL);
            writecmd(0x0008, 0xFF);
            recvinit();
        }
    }

    if (sendUpdate) {
        sendUpdate = FALSE;
        sendsub0();
    } else {
        EXIUnlock(0);
        if (lockUpdate) {
            lockUpdate = FALSE;
            OSWakeupThread(&threadQ);
        }
    }

    OSRestoreInterrupts(disabled);
}
