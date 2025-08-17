#include "includes.h"
#include "VTerm.h"

int count = 0; //this is for counting switching times
OS_STK Task1Stk[64];
OS_STK Task2Stk[64];
OS_STK StartTaskStk[64];

void TaskMgtTask1(void *pdata);
void TaskMgtTask2(void *pdata);
void TaskMgtStartTask(void *pdata);

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR |= BIT0;
    P1OUT &= ~BIT0; // LED off initially
    P1SEL &= ~BIT0;

    VTermInit(1);

    WDTCTL = WDTPW | WDTCNTCL | WDT_MDLY_32;
    SFRIE1 |= BIT0; // Enable WDT interrupt

    VTermPrintf("---demo ucos-ii task management---\n");
    VTermPrintf("ID\tSTATUS\tTICKS\n");

    OSInit();

    OSTaskCreate(TaskMgtStartTask, (void *)0, &StartTaskStk[63], 4);

    OSStart();
    return 0;
}


void TaskMgtStartTask(void *pdata) {
    OSTaskCreate(TaskMgtTask1, (void *)0, &Task1Stk[63], 5);
    OSTaskCreate(TaskMgtTask2, (void *)0, &Task2Stk[63], 6);
    OSTaskSuspend(4);
}

void TaskMgtTask1(void *pdata) {
    while(1) {
        VTermPrintf("Task1 RUN \n");
        P1OUT |= BIT0;    // Toggle LED
        OSTaskSuspend(5);          // Suspend Task1
    }
}


void TaskMgtTask2(void *pdata) {
    while(1) {
        VTermPrintf("Task2-1 RUN \n");
        OSTaskResume(5);
        P1OUT &= ~BIT0;    // Toggle LED
        VTermPrintf("Task2-2 RUN \n");
        OSTimeDly(100);    // Delay 1000 ticks = 1 second
    }
}
