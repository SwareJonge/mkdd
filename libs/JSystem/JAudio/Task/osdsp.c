#include <dolphin/os.h>
#include "JSystem/JAudio/dsp.h"

DSPTaskInfo *DSPAddTask(DSPTaskInfo *task)
{
    int interrupts;
    interrupts = OSDisableInterrupts();
    __DSP_insert_task(task);
    task->state = DSP_TASK_STATE_INIT;
    task->flags = DSP_TASK_FLAG_ATTACHED;
    OSRestoreInterrupts(interrupts);
    return task;
}

void DSPAddPriorTask(DSPTaskInfo *task)
{
    int interrupts;
    if (DSP_prior_task != nullptr)
    {
        OSReport("Already inited prior DSP task\n");
        return;
    }
    interrupts = OSDisableInterrupts();
    DSP_prior_task = task;
    task->state = DSP_TASK_STATE_INIT;
    task->flags = DSP_TASK_FLAG_ATTACHED;
    __DSP_boot_task(task);
    OSRestoreInterrupts(interrupts);
}