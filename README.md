
RTOS Scheduling Variations 
Demonstrating Priority Sync, Inheritance, Inversion, Task Management, and LED Scheduling on MSP430 with µC/OS-II

This repository contains a collection of real-time operating system (RTOS) scheduling examples using µC/OS-II on MSP430 microcontrollers. Each example highlights a different RTOS concept, including priority synchronization, priority inheritance, priority inversion, task management, and LED-based task scheduling.

📁 Repository Structure
File	Description	Key Concept
priority_sync_test.c	Synchronizes two tasks (T1 and T2) using a binary semaphore to enforce execution order.	Priority Synchronization
priority_inherit_test.c	Demonstrates priority inheritance with a mutex shared across three tasks (T0, T1, T2).	Priority Inheritance
priority_invert_test.c	Simulates priority inversion with semaphore contention between high/low-priority tasks.	Priority Inversion
task_mang_test.c	Shows task suspension/resumption and LED toggling between Task1 and Task2.	Task Management
task_sche_led_test.c	Blinks an LED using two tasks with different delays (TaskA and TaskB).	Time-Sliced Scheduling

🔍 Key Concepts Explained
1. Priority Synchronization (priority_sync_test.c)
Uses a binary semaphore to ensure T1 executes before T2.

T1 posts the semaphore after a delay, allowing T2 to proceed.

Output:

text
T11: Start → T11: Done → T22: RUN → T22: Done
2. Priority Inheritance (priority_inherit_test.c)
A mutex prevents priority inversion by temporarily boosting the priority of a low-priority task (T0) holding the mutex when a high-priority task (T2) waits.

Tasks print their state (Wait, Access, Release).

3. Priority Inversion (priority_invert_test.c)
A semaphore is contested by T0 (low-priority) and T2 (high-priority), causing inversion when T1 (medium-priority) preempts T0.

Output:

text
T0 (low) → T1 (medium) → T2 (high) blocked → T0 finishes → T2 runs
4. Task Management (task_mang_test.c)
Task2 suspends/resumes Task1 while toggling an LED.

Demonstrates dynamic task control with OSTaskSuspend()/OSTaskResume().

5. LED Scheduling (task_sche_led_test.c)
Two tasks (TaskA and TaskB) toggle an LED at different rates (10ms and 20ms).

Shows time-sliced scheduling with OSTimeDly().

🛠️ Hardware Setup
MCU: MSP430 (e.g., MSP430G2553)

IDE: Code Composer Studio (CCS) or MSP430 GCC

