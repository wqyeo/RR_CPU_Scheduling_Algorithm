// Optimal Round Robin CPU Scheduling Algorithm using Manhattan Distance
#include <float.h>
#include <stdio.h>

#include "process.h"
#include "round_robin.h"



void manhattan_round_robin(Process *processes, int processesSize){
    float max = FLT_MIN;
    float min = FLT_MAX;

    int i;
    for (i = 0; i < processesSize; i++) {
        if (processes[i].burstTime > max) {
            max = processes[i].burstTime;
        }
        if (processes[i].burstTime < min) {
            min = processes[i].burstTime;
        }
    }

    float quantumTime = max - min;
    if (quantumTime == 0){
        // All process burst time is the same.
        quantumTime = processes[0].burstTime;
    }

    printf("\nQuantum Time (Manhattan Distance): %.2f\n", quantumTime);

    round_robin(processes, processesSize, quantumTime);
}
