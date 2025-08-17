#include "includes.h"
#include "VTerm.h"

OS_EVENT *mutex;

#define TASK_STK_SIZE 64
#define N_TASKS 3

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];
OS_STK StartTaskStk[TASK_STK_SIZE];
INT8U TaskData[N_TASKS];

void Task(void *pdata);
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

void Task(void *pdata) {
    INT8U err;
    INT8U id = (INT8U)(INT32U)pdata;
    while (1) {
        VTermPrintf("T%d: Wait\n", id);
        OSMutexPend(mutex, 0, &err);
        VTermPrintf("T%d: Access\n", id);
        OSTimeDly(200);
        OSMutexPost(mutex);
        VTermPrintf("T%d: Release\n", id);
        OSTimeDly(150 *( id + 1));
    }
}

static void PI_StartTask(void *pdata) {
    INT8U err;
    mutex = OSMutexCreate(1, &err);

    OSTaskCreate(Task, (void *)2, &TaskStk[2][TASK_STK_SIZE - 1], 6);
    OSTaskCreate(Task, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 4);
    OSTaskCreate(Task, (void *)1, &TaskStk[1][TASK_STK_SIZE - 1], 5);


    OSTaskSuspend(2);
}
