


#include "includes.h"
#include "VTerm.h"

#define TASK_STK_SIZE 64        // Size of each task's stacks (# of WORDs)
#define N_TASKS 3               // Number of tasks

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE]; // Tasks stacks
OS_STK StartTaskStk[TASK_STK_SIZE];     // Start task stack
OS_EVENT *semaphore;                        // Mutex for shared resource

// Task management
void PI_Task0(void *pdata);
void PI_Task1(void *pdata);
void PI_Task2(void *pdata);
void PI_StartTask(void *pdata);



int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog initially

    // Set P1.0 as output for LED (assuming LED is on P1.0)
    P1DIR |= BIT0;
    P1OUT &= ~BIT0; // LED off initially
    P1SEL &= ~BIT0;


    // LCD init
    VTermInit(1);

    // Set up watchdog timer for 32ms ticks
    WDTCTL = WDTPW | WDTCNTCL | WDT_MDLY_32;
    SFRIE1 |= BIT0; // Enable WDT interrupt

    VTermPrintf("---demo ucos-ii priority inherit---\n");
    VTermPrintf("ID\tSTATUS\tTICKS\n");

    OSInit();

    // Create the start task
    OSTaskCreate(PI_StartTask, (void *)0, &StartTaskStk[TASK_STK_SIZE - 1], 2);

    OSStart();
    return 0;
}


void PI_StartTask(void *pdata) {
    //INT8U err;

    // Create mutex for shared resource
    semaphore = OSSemCreate(1);

    // Create tasks T0, T1, T2
    OSTaskCreate(PI_Task0, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 4); // T0: priority 3
    OSTaskCreate(PI_Task1, (void *)0, &TaskStk[1][TASK_STK_SIZE - 1], 5); // T1: priority 4
    OSTaskCreate(PI_Task2, (void *)0, &TaskStk[2][TASK_STK_SIZE - 1], 6); // T2: priority 5

    OSTaskSuspend(2);

}





void PI_Task0(void *pdata) {
    INT8U err;
    while (1) {
        VTermPrintf("T0\n");
        OSTimeDly(200); // Hold resource for 200 ticks (~6.4s)
        VTermPrintf("T0\tACCESSING\n");
        OSSemPend(semaphore, 0, &err); // Acquire semaphore
        OSTimeDly(10);
        OSSemPost(semaphore); // Release semaphore
        VTermPrintf("T0\tIDLE\n");
    }
}

void PI_Task1(void *pdata) {
    while (1) {
        VTermPrintf("T1\tRUNNING\n");
        OSTimeDly(100); // Delay 100 ticks (~3.2s) to yield
        VTermPrintf("T1\tIDLE\n");
    }
}

void PI_Task2(void *pdata) {
    INT8U err;
    while (1) {
        VTermPrintf("T2\n");
        OSSemPend(semaphore, 0, &err); // Acquire semaphore
        VTermPrintf("T2\tACCESSING\n");
        OSTimeDly(200); // Hold resource for 200 ticks (~6.4s)
        OSSemPost(semaphore); // Release semaphore
        VTermPrintf("T2\tIDLE\n");
    }
}






















