#include "dolphin/os.h"
#include "dolphin/stl.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "types.h"

// WIP
JSUList<JKRThread> JKRThread::sThreadList(false);
JSUList<JKRTask> JKRTask::sTaskList;

JKRThreadSwitch* JKRThreadSwitch::sManager;
u32 JKRThreadSwitch::sTotalCount;
//u64 JKRThreadSwitch::sTotalStart; // Unused

JKRThread::JKRThread(u32 stack_size, int message_count, int param_3) : mThreadListLink(this) {
    JKRHeap* heap = JKRHeap::findFromRoot(this);
    if (heap == NULL) {
        heap = JKRHeap::getSystemHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_3);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(JKRHeap* heap, u32 stack_size, int message_count, int param_4)
    : mThreadListLink(this) {
    if (heap == NULL) {
        heap = JKRHeap::getCurrentHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_4);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(OSThread* thread, int message_count) : mThreadListLink(this) {
    mHeap = NULL;
    mThreadRecord = thread;
    mStackSize = (u32)thread->stackEnd - (u32)thread->stackBase;
    mStackMemory = thread->stackBase;

    setCommon_mesgQueue(JKRHeap::getSystemHeap(), message_count);
}

JKRThread::~JKRThread()
{
	sThreadList.remove(&mThreadListLink);
	if (mHeap != nullptr) {
		if (!OSIsThreadTerminated(mThreadRecord)) {
			OSDetachThread(mThreadRecord);
			OSCancelThread(mThreadRecord);
		}
		JKRHeap::free(mStackMemory, mHeap);
		JKRHeap::free(mThreadRecord, mHeap);
	}
	JKRHeap::free(mMesgBuffer, nullptr);
}

void JKRThread::setCommon_mesgQueue(JKRHeap* heap, int msgCount)
{
	mMessageCount  = msgCount;
	mMesgBuffer = (OSMessage*)JKRHeap::alloc(mMessageCount << 2, 0, heap);
    JUT_ASSERT(130, mMesgBuffer);

	OSInitMessageQueue(&mMessageQueue, (void**)mMesgBuffer, mMessageCount);
	JKRThread::sThreadList.append(&mThreadListLink);
	mCurrentHeap = nullptr;
	mCurrentHeapError = nullptr;
}

BOOL JKRThread::setCommon_heapSpecified(JKRHeap* heap, unsigned long stackSize, int threadPriority)
{
	mHeap      = heap;
	mStackSize = stackSize & ~0x1F;
	mStackMemory     = JKRHeap::alloc(mStackSize, 0x20, mHeap);
    JUT_ASSERT(164, mStackMemory);

	mThreadRecord    = (OSThread*)JKRHeap::alloc(sizeof(OSThread), 0x20, mHeap);
    JUT_ASSERT(168, mThreadRecord);
	return OSCreateThread(mThreadRecord, &JKRThread::start, this, (void*)((u32)mStackMemory + mStackSize), mStackSize, threadPriority, 1);
}

void* JKRThread::start(void* thread) { return static_cast<JKRThread*>(thread)->run(); }

JKRThread* JKRThread::searchThread(OSThread* thread) {
    JSUList<JKRThread>& threadList = getList();
    JSUListIterator<JKRThread> iterator;
    for (iterator = threadList.getFirst(); iterator != threadList.getEnd(); ++iterator) {
        if (iterator->getThreadRecord() == thread) {
            return iterator.getObject();
        }
    }

    return nullptr;
}

JKRThreadSwitch::JKRThreadSwitch(JKRHeap* param_0) {
    /*mHeap = param_0;
    OSSetSwitchThreadCallback(JKRThreadSwitch::callback);
    this->field_0xC = 0;
    this->field_0x10 = 1;
    this->field_0x18 = 0;
    sTotalCount = 0;
    data_804513BC = 0;
    sTotalStart = 0;
    this->field_0x20 = 0;
    this->field_0x24 = 0;
    mSetNextHeap = true;*/
}

JKRThreadSwitch* JKRThreadSwitch::createManager(JKRHeap* heap) {
    JUT_ASSERT(0x157, sManager == 0);

    if (!heap) {
        heap = JKRGetCurrentHeap();
    }

    sManager = new (heap, 0) JKRThreadSwitch(heap);
    return sManager;
}

JKRThread * JKRThreadSwitch::enter(JKRThread * thread, int id) {
    if(thread == nullptr) {
        thread = nullptr;        
    }
    else {
        JKRThread * nextThread = JKRThread::searchThread(thread->getThreadRecord());
        if(nextThread != nullptr)
            thread = nextThread;
        
        JKRThread::TLoad * loadInfo = thread->getLoadInfo();
        loadInfo->clear();
        loadInfo->setValid(true);
        loadInfo->setId(id);
    }
    return thread;
}

void JKRThreadSwitch::callback(OSThread* current, OSThread* next) {
    if (mUserPreCallback) {
        (*mUserPreCallback)(current, next);
    }

    sTotalCount = sTotalCount + 1;

    JKRHeap* next_heap = NULL;
    JSUList<JKRThread>& threadList = JKRThread::getList();
    JSUListIterator<JKRThread> iterator;
    for (iterator = threadList.getFirst(); iterator != threadList.getEnd(); ++iterator) {
        JKRThread* thread = iterator.getObject();

        if (thread->getThreadRecord() == current) {
            thread->setCurrentHeap(JKRHeap::getCurrentHeap());
            JKRThread::TLoad* loadInfo = thread->getLoadInfo();
            if (loadInfo->isValid()) {
                loadInfo->addCurrentCost();
            }
        }

        if (thread->getThreadRecord() == next) {
            JKRThread::TLoad* loadInfo = thread->getLoadInfo();
            if (loadInfo->isValid()) {
                loadInfo->setCurrentTime();
                loadInfo->incCount();
            }

            if (sManager->mSetNextHeap) {
                next_heap = thread->getCurrentHeap();
                if (!next_heap) {
                    next_heap = JKRHeap::getCurrentHeap();
                } else if (JKRHeap::getRootHeap()->isSubHeap(next_heap)) {
                    continue;
                } else {
                    switch (thread->getCurrentHeapError()) {
                    case 0:
                        JUT_PANIC(0x1fc, "JKRThreadSwitch: currentHeap destroyed.");
                        break;
                    case 1:
                        JUTWarningConsole("JKRThreadSwitch: currentHeap destroyed.\n");
                        next_heap = JKRHeap::getCurrentHeap();
                        break;
                    case 2:
                        next_heap = JKRHeap::getCurrentHeap();
                        break;
                    case 3:
                        next_heap = JKRHeap::getSystemHeap();
                        break;
                    }
                }
            }
        }
    }

    if (next_heap) {
        next_heap->becomeCurrentHeap();
    }

    if (mUserPostCallback) {
        (*mUserPostCallback)(current, next);
    }
}

void JKRThreadSwitch::draw(JKRThreadName_* thread_name_list, JUTConsole* console) {
    const char* print_0 = " total: switch:%3d  time:%d(%df)\n";
    const char* print_1 = " -------------------------------------\n";

    if (!console) {
/*#if DEBUG
        OSReport(print_0, getTotalCount(), (int)this->field_0x18, this->field_0x10);
        OSReport(print_1);
#endif*/
    } else {
        console->clear();
        console->print_f(print_0, getTotalCount(), (int)this->field_0x18, this->field_0x10);
        console->print(print_1);
    }

    JSUList<JKRThread>& threadList = JKRThread::getList();
    JSUListIterator<JKRThread> iterator;
    for (iterator = threadList.getFirst(); iterator != threadList.getEnd(); ++iterator) {
        JKRThread* thread = iterator.getObject();
        JKRThread::TLoad* loadInfo = thread->getLoadInfo();

        if (loadInfo->isValid()) {
            char* thread_print_name = NULL;
            if (thread_name_list) {
                JKRThreadName_* thread_name = thread_name_list;
                for (; thread_name->name; thread_name++) {
                    if (thread_name->id == loadInfo->getId()) {
                        thread_print_name = thread_name->name;
                        break;
                    }
                }
            }

            if (!thread_print_name) {
                char buffer[16];
                sprintf(buffer, "%d", loadInfo->getId());
                thread_print_name = buffer;
            }

            u32 switch_count = loadInfo->getCount();
            float cost_per_0x18 = loadInfo->getCost() / (float)this->field_0x18;

            u32 cost_int = (u32)(cost_per_0x18 * 100.0f);
            u32 cost_float = (u32)(cost_per_0x18 * 1000.0f) % 10;
            if (!console) {
/*#if DEBUG
                OSReport(" [%10s] switch:%5d  cost:%2d.%d%%\n", thread_print_name, switch_count,
                         cost_int, cost_float);
#endif*/
            } else {
                console->print_f(" [%10s] switch:%5d  cost:%2d.%d%%\n", thread_print_name,
                                 switch_count, cost_int, cost_float);
            }
        }
    }
}

JKRTask::JKRTask(int msgCount, int threadPriority, u32 stackSize)
    : JKRThread(stackSize, msgCount, threadPriority)
    , _7C(this)
    , _94(nullptr)
{
	// UNUSED FUNCTION
	OSResumeThread(mThreadRecord);
}

JKRTask::~JKRTask() { sTaskList.remove(&_7C); }

JKRTask* JKRTask::create(int msgCount, int threadPriority, unsigned long stackSize, JKRHeap* heap)
{
	if (heap == nullptr) {
		heap = JKRHeap::sCurrentHeap;
	}
	JKRTask* task = new (heap, 0) JKRTask(msgCount, threadPriority, stackSize);
	if (task == nullptr) {
		return nullptr;
	}
	task->_8C = new (heap, 0) Message[msgCount];
	task->_90 = msgCount;
	if (task->_8C == nullptr) {
		delete task;
		return nullptr;
	}
	for (int i = 0; i < msgCount; i++) {
		task->_8C[i]._00 = nullptr;
	}
	sTaskList.append(&task->_7C);
	return task;
}

void* JKRTask::run()
{
	Message* msg;
    OSInitFastCast();
	while (true) {
		OSReceiveMessage(&mMessageQueue, (void**)&msg, OS_MESSAGE_BLOCK);
		if (msg->_00 != nullptr) {
			msg->_00(msg->_04);
			if (_94 != nullptr) {
				OSSendMessage(_94, msg->_08, OS_MESSAGE_NOBLOCK);
			}
		}
		msg->_00 = nullptr;
	}
}

JKRTask::Message* JKRTask::searchBlank()
{
	// UNUSED FUNCTION
	for (int i = 0; i < _90; i++) {
		if (_8C[i]._00 == nullptr) {
			return _8C + i;
		}
	}
	return nullptr;
}

bool JKRTask::request(RequestCallback callback, void* p2, void* p3)
{
	Message* msg = searchBlank();
	if (msg == nullptr) {
		return false;
	}
	msg->_00        = callback;
	msg->_04        = p2;
	msg->_08        = p3;
	bool sendResult = (OSSendMessage(&mMessageQueue, msg, OS_MESSAGE_NOBLOCK) != FALSE);
	if (sendResult == false) {
		msg->_00 = nullptr;
	}
	return sendResult;
}