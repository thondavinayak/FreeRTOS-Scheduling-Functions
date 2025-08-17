#include "includes.h"
#include "VTerm.h"

OS_EVENT *mutex;

#define TASK_STK_SIZE 64
#define N_TASKS 3

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
OS_STK StartTaskStk[TASK_STK_SIZE];
INT8U TaskData[N_TASKS];

void T0(void *pdata);
void T1(void *pdata);
void T2(void *pdata);
static void PI_StartTask(void *pdata);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR = 0x0;
    P2DIR = 0x0;
    P3DIR = 0x0;
    P4DIR = 0x0;
    P5DIR = 0x0;
    P6DIR = 0x0;

    VTermInit(1);
    WDTCTL = WDT_MDLY_32;
    SFRIE1 |= BIT0;

    VTermPrintf("---demo ucos-ii priority inherit---\n\n");

    OSInit();
    OSTaskCreate(PI_StartTask, (void *)0x12345678L, (void *)&StartTaskStk[TASK_STK_SIZE - 1], 2);
    OSStart();
}

void T0(void *pdata) {
    INT8U err;
    while (1) {
        VTermPrintf("T0: Wait\n");
        OSMutexPend(mutex, 0, &err);
        VTermPrintf("T0: Access\n");
        OSTimeDly(200);
        OSMutexPost(mutex);
        VTermPrintf("T0: Release\n");
        OSTimeDly(150);
    }
}

void T1(void *pdata) {
    INT8U err;
    while (1) {
        //OSTimeDly(2);
        VTermPrintf("T1: Wait\n");
        OSMutexPend(mutex, 0, &err);
        VTermPrintf("T1: Access\n");
        OSTimeDly(200);
        OSMutexPost(mutex);
        VTermPrintf("T1: Release\n");
        OSTimeDly(300);
    }
}

void T2(void *pdata) {
    INT8U err;
    while (1) {
        //OSTimeDly(4);
        VTermPrintf("T2: Wait \n");
        OSMutexPend(mutex, 0, &err);
        VTermPrintf("T2: Access\n");
        OSTimeDly(200);
        OSMutexPost(mutex);
        VTermPrintf("T2: Release\n");
        OSTimeDly(450);
    }
}

static void PI_StartTask(void *pdata) {
    INT8U err;
    mutex = OSMutexCreate(1, &err);

    OSTaskCreate(T0, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 4);
    OSTaskCreate(T1, (void *)1, &TaskStk[1][TASK_STK_SIZE - 1], 5);
    OSTaskCreate(T2, (void *)2, &TaskStk[2][TASK_STK_SIZE - 1], 6);

    OSTaskSuspend(2);
}
