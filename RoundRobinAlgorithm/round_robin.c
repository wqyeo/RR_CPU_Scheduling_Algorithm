// CPU Scheduling Method : Round Robin
#include <stdio.h>
#include "process.h"

void round_robin(Process *processes, int processesSize, float timeQuantum) {
    int i, j;
    float currentTime = 0;
    float remainingTime[processesSize];
    float waitingTime[processesSize];
    float turnaroundTime[processesSize];
    float responseTime[processesSize];

    int contextSwitch = 0;

    for (i = 0; i < processesSize; i++) {
        remainingTime[i] = (int) processes[i].burstTime;
        waitingTime[i] = 0;
        turnaroundTime[i] = 0;
        responseTime[i] = 0;
    }

    // This loop simulates the Round Robin on all of the processes.
    while (1) {
        int allProcessesDoneFlag = 1;
        for (i = 0; i < processesSize; i++) {
            // Ensure the response time for this process is never negative.
            responseTime[i] = ((i  * timeQuantum) - processes[i].arrivalTime >= 0) ? ((i  * timeQuantum) - processes[i].arrivalTime) : 0;

            // Unfinished process...
            if (remainingTime[i] > 0) {
                allProcessesDoneFlag = 0;
                if (remainingTime[i] > timeQuantum) {
                    // Process won't finish by this time quantum.
                    // Increment current time by the time quantum, then decrease the process's remaining time.
                    currentTime += timeQuantum;
                    remainingTime[i] -= timeQuantum;
                } else {
                    // Process will finish by this time quantum, increment current time by remaining time.
                    // Then, calculate the times for this process...
                    currentTime += remainingTime[i];
                    waitingTime[i] = currentTime - processes[i].arrivalTime - processes[i].burstTime;
                    turnaroundTime[i] = currentTime - processes[i].arrivalTime;
                    remainingTime[i] = 0;
                }
                ++contextSwitch;
            }
        }
        if (allProcessesDoneFlag == 1) {
            break;
        }
    }

    // Print out a table of the times for each of the process.
    printf("\nPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (i = 0; i < processesSize; i++) {
        printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", processes[i].name, processes[i].burstTime, waitingTime[i], turnaroundTime[i], responseTime[i]);
    }

    // Calculate average of times, then print out.
    float avgWaitingTime = 0.0, avgTurnaroundTime = 0.0, avgResponseTime = 0.0;
    for (i = 0; i < processesSize; i++) {
        avgWaitingTime += waitingTime[i];
        avgTurnaroundTime += turnaroundTime[i];
        avgResponseTime += responseTime[i];
    }
    avgWaitingTime /= processesSize;
    avgTurnaroundTime /= processesSize;
    avgResponseTime /= processesSize;

    printf("\nAverage Waiting Time: %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
    printf("Average Response Time: %.2f\n", avgResponseTime);
    printf("Context Switch: %d\n", contextSwitch);
}
