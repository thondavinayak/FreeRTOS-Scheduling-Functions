#include "includes.h"
#include <msp430.h>

#define TASK_STK_SIZE 64
#define LED_PIN BIT0

OS_STK TaskStartStkA[TASK_STK_SIZE];
OS_STK TaskStartStkB[TASK_STK_SIZE];

void TaskStartA(void *pdata);
void TaskStartB(void *pdata);

void main(void)
{
    //WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTIS1 | WDTIS0;  // WDT interval ~1ms
    WDTCTL = WDTPW | WDTCNTCL | WDT_MDLY_32;

    SFRIE1 |= BIT0;                                        // Enable WDT interrupt

    // Configure P1.0 as output
    P1DIR |= LED_PIN;
    P1OUT &= ~LED_PIN;
    P1SEL &= ~LED_PIN;

    OSInit();

    OSTaskCreate(TaskStartA, (void *)0, &TaskStartStkA[TASK_STK_SIZE - 1], 5);

    OSTaskCreate(TaskStartB, (void *)0, &TaskStartStkB[TASK_STK_SIZE - 1], 6);

    OSStart();

}

void TaskStartA(void *pdata)
{
    while(1)
    {
        P1OUT |= LED_PIN;    // Toggle LED
        OSTimeDly(10);    // Delay 1000 ticks = 1 second
    }
}


void TaskStartB(void *pdata)
{
    while(1)
    {
        P1OUT &= ~LED_PIN;    // Toggle LED
        OSTimeDly(20);    // Delay 1000 ticks = 1 second
    }
}

