// Optimal Round Robin CPU Scheduling Algorithm using Manhattan Distance
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "../Models/process.h"
#include "../Models/round_robin_result.h"
#include "round_robin.h"

RoundRobinResult manhattan_round_robin(Process *processes, int processesSize, char* grouping){
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

    // Manhattan round robin has the same exact algorithm as a normal round robin,
    // the difference is the quantum time used.
    RoundRobinResult result = round_robin(processes, processesSize, quantumTime, grouping);
    strcpy(result.roundRobinUsed, "Round Robin Manhattan Distance");
    return result;
}
