// CPU Scheduling Method : Round Robin
#include <stdio.h>

#include "process.h"
#include "process_result.h"
#include "round_robin_result.h"

RoundRobinResult round_robin(Process *processes, int processesSize, float timeQuantum) {
    int i, j;
    float remainingTime[processesSize];

    RoundRobinResult result;
    result.processResults = (ProcessResult*) malloc(processesSize * sizeof(ProcessResult));
    result.processesSize = processesSize;

    result.totalTime = 0;
    result.contextSwitch = 0;
    result.timeQuantum = timeQuantum;

    // init
    for (i = 0; i < processesSize; i++) {
        remainingTime[i] = processes[i].burstTime;
        result.processResults[i].waitingTime = 0.0f;
        result.processResults[i].turnaroundTime = 0.0f;
        result.processResults[i].responseTime = 0.0f;
        result.processResults[i].process = processes[i];
    }

    // This loop simulates the Round Robin on all of the processes.
    while (1) {
        int allProcessesDoneFlag = 1;
        for (i = 0; i < processesSize; i++) {
            // Ensure the response time for this process is never negative.
            result.processResults[i].responseTime = ((i  * timeQuantum) - processes[i].arrivalTime >= 0) ? ((i  * timeQuantum) - processes[i].arrivalTime) : 0;

            // Unfinished process...
            if (remainingTime[i] > 0) {
                allProcessesDoneFlag = 0;
                if (remainingTime[i] > timeQuantum) {
                    // Process won't finish by this time quantum.
                    // Increment current time by the time quantum, then decrease the process's remaining time.
                    result.totalTime += timeQuantum;
                    remainingTime[i] -= timeQuantum;
                } else {
                    // Process will finish by this time quantum, increment current time by remaining time.
                    // Then, calculate the times for this process...
                    result.totalTime += remainingTime[i];
                    result.processResults[i].waitingTime = result.totalTime  - processes[i].arrivalTime - processes[i].burstTime;
                    result.processResults[i].turnaroundTime = result.totalTime  - processes[i].arrivalTime;
                    remainingTime[i] = 0;
                }
                result.contextSwitch += 1;
            }
        }
        if (allProcessesDoneFlag == 1) {
            break;
        }
    }

    // Calculate average of times
    result.avgResponseTime = 0.0f; result.avgTurnaroundTime = 0.0f; result.avgWaitingTime = 0.0f;
    for (i = 0; i < processesSize; i++) {
        result.avgWaitingTime += result.processResults[i].waitingTime;
        result.avgTurnaroundTime += result.processResults[i].turnaroundTime;
        result.avgResponseTime += result.processResults[i].responseTime;
    }
    result.avgWaitingTime /= processesSize;
    result.avgTurnaroundTime /= processesSize;
    result.avgResponseTime /= processesSize;

    strcpy(result.roundRobinUsed, "Round Robin");

    return result;
}
