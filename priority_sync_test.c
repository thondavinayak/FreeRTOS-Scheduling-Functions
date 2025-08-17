#include "includes.h"
#include "VTerm.h"

#define STACK_SIZE 128

OS_STK T1_Stk[STACK_SIZE];
OS_STK T2_Stk[STACK_SIZE];
OS_STK StartTaskStk[STACK_SIZE];

#define PRIO_T1     5
#define PRIO_T2     6
#define PRIO_START  4

OS_EVENT *BinSem;

void T1(void *pdata);
void T2(void *pdata);
void StartTask(void *pdata);

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR = P2DIR = P3DIR = P4DIR = P5DIR = P6DIR = 0x0;

    VTermInit(1);
    WDTCTL = WDT_MDLY_32;
    SFRIE1 |= BIT0;

    VTermPrintf("--- T1 before T2 | Periodic Tasks (175ms) ---\n");

    OSInit();

    OSTaskCreate(StartTask, NULL, &StartTaskStk[STACK_SIZE - 1], PRIO_START);

    OSStart();
}

void StartTask(void *pdata) {
    BinSem = OSSemCreate(0);

    OSTaskCreate(T1, NULL, &T1_Stk[STACK_SIZE - 1], PRIO_T1);
    OSTaskCreate(T2, NULL, &T2_Stk[STACK_SIZE - 1], PRIO_T2);

    OSTaskSuspend(PRIO_START);
}

void T1(void *pdata) {
    while (1) {
        VTermPrintf("T11: Start\n");
        OSTimeDlyHMSM(0, 0, 0, 5);

        VTermPrintf("T11: Done\n");
        OSSemPost(BinSem);

        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

void T2
(void *pdata) {
    INT8U err;
    while (1) {
        OSSemPend(BinSem, 0, &err);

        VTermPrintf("T22: RUN\n");
        OSTimeDlyHMSM(0, 0, 0, 6);

        VTermPrintf("T22: Done\n");

        OSTimeDlyHMSM(0, 0, 0, 75);
    }
}

