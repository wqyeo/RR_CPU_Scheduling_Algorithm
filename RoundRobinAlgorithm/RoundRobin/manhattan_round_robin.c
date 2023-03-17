// Optimal Round Robin CPU Scheduling Algorithm using Manhattan Distance
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "../Models/process.h"
#include "../Models/round_robin_result.h"
#include "round_robin.h"

RoundRobinResult manhattan_round_robin(Process *processes, int processesSize, char* grouping)
{
    int i;

    float maxBurstTime = FLT_MIN;
    float minBurstTime = FLT_MAX;
    // Find the max and min burst time from the processes.
    for (i = 0; i < processesSize; i++)
    {
        if (processes[i].burstTime > maxBurstTime)
        {
            maxBurstTime = processes[i].burstTime;
        }
        if (processes[i].burstTime < minBurstTime)
        {
            minBurstTime = processes[i].burstTime;
        }
    }

    // Quantum time is max burst time - min burst time.
    float quantumTime = maxBurstTime - minBurstTime;

    // If all processes burst time is the same,
    // just set time quantum to the burst time.
    if (quantumTime == 0)
    {
        quantumTime = processes[0].burstTime;
    }

    // Manhattan round robin has the same exact algorithm as a traditional round robin,
    // the difference is the quantum time used.
    RoundRobinResult result = round_robin(processes, processesSize, quantumTime, grouping);
    strcpy(result.roundRobinUsed, "Round Robin Manhattan Distance");
    return result;
}
