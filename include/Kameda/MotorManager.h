#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#include "JSystem/JKernel/JKRDisposer.h"

#include "types.h"

enum MotorType {

};

class MotorManager : JKRDisposer { // Autogenerated
public:
    // Global
    MotorManager(JKRHeap *);                // 0x80130e58
    virtual ~MotorManager();                // 0x80130ee0
    void reset();                           // 0x80130f48
    void exec();                            // 0x80130fc0
    void setMotor(MotorType, int);          // 0x8013103c
    void setAwardMotor(MotorType);          // 0x80131218
    void setPowerMotor(int, float, u8, u8); // 0x801312e8

    static MotorManager * getManager();
    
    static int mCountFrame;                 // 0x803fb640
    static MotorManager *mThis;             // 0x80416280

private:
    u8 _0[8];
};

#endif