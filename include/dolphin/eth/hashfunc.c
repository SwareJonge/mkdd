#include <dolphin/types.h>

static u32 hashfunc(u32 seccode) {
    u8 p;
    u8 q;
    u8 r;
    u8 s;
    u8 a[sizeof(seccode)];
    u8 dev[sizeof(__devid)];

    memcpy(a, &seccode, sizeof(seccode));
    memcpy(dev, &__devid, sizeof(__devid));

    p = (a[0] + a[1] * 0xC1 + 0x18 + __revid) ^ (a[3] * a[2] + 0x90);
    q = (a[1] + a[2] + 0x90) ^ (p - 0xC1 + a[0]);
    r = (a[2] + 0xC8) ^ (p + ((dev[0] + __revid * 0x23) ^ 0x19));
    s = (a[0] + 0xC1) ^ (a[3] + ((dev[1] + 0xC8) ^ 0x90));

    return (p << 24) | (q << 16) | (r << 8) | s;
}
