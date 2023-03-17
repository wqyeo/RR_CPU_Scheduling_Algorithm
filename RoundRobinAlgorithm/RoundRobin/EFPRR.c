// CPU Scheduling Method : EIGHTY FIVE PERCENTILE ROUND ROBIN CPU SCHEDULING ALGORITHM
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"
#include "../Util/color_print.h"

#include "EFPRR.h"
#include "round_robin.h"

// Used to sort processes in the ready queue by burst time.
int compare_ready_processes(const void *a, const void *b)
{
    ReadyProcess *p1 = (ReadyProcess *)a;
    ReadyProcess *p2 = (ReadyProcess *)b;

    return p1->burstTime - p2->burstTime;
}


// https://stackoverflow.com/questions/5989191/compare-two-floats
// Because floats are not precise.
int areEqual(float a, float b, float epsilon) {
    return fabs(a - b) < epsilon;
}

RoundRobinResult EFPRR(Process *processes, int processesSize, char* grouping)
{
    RoundRobinResult result;
    int i, j;

    // This array will be used to keep track of what processes we have added into the ready queue before,
    // which hasn't been added yet.
    int addedProcessFlag[processesSize];

    int readyQueueSize = 0;
    ReadyProcess* readyQueue = (ReadyProcess*) malloc(sizeof(ReadyProcess) * processesSize);

    strcpy(result.grouping, grouping);
    result.processesSize = processesSize;
    result.processResults = (ProcessResult*) malloc(sizeof(ProcessResult) * processesSize);
    // Start at the lowest arrival time, simulate idling until first process comes.
    result.totalTime = get_lowest_arrival_time(processes, processesSize);
    result.contextSwitch = 0;

    for (i = 0; i < processesSize; ++i) {
        result.processResults[i].waitingTime = 0.0f;
        result.processResults[i].turnaroundTime = 0.0f;
        result.processResults[i].responseTime = -1.0f;
        result.processResults[i].process = processes[i];

        // Process arrives at the start, add to ready queue.
        if (processes[i].arrivalTime <= result.totalTime) {
            strcpy(readyQueue[readyQueueSize].processName, processes[i].name);
            readyQueue[readyQueueSize].remainingTime = processes[i].burstTime;
            readyQueue[readyQueueSize].burstTime = processes[i].burstTime;
            ++readyQueueSize;

            // Mark the position of the process has been added.
            addedProcessFlag[i] = 1;
        } else {
            addedProcessFlag[i] = 0;
        }
    }

    qsort(readyQueue, readyQueueSize, sizeof(ReadyProcess), compare_ready_processes);

    char lastProcess[MAX_NAME_LEN] = "";

    float currentTimeQuantum = 0.0f;
    result.timeQuantumUsed = 0;
    while (1) {
        int processExecutedFlag = 0;
        for (i = 0; i < readyQueueSize; i++) {

            // Calculate time quantum by (Average Burst Time / Number of processes) * 0.85f;
            float newTimeQuantum = 0.0f;
            int numOfProcesses = 0;
            for (j = 0; j < readyQueueSize; ++j){
                if (readyQueue[j].remainingTime > 0.0f) {
                    newTimeQuantum += readyQueue[j].burstTime;
                    ++numOfProcesses;
                }
            }
            newTimeQuantum /= numOfProcesses;
            newTimeQuantum *= 0.85f;

            // Time quantum changed, record it down.
            if (areEqual(currentTimeQuantum, newTimeQuantum, 0.0001) == 0){
                result.timeQuantums[result.timeQuantumUsed] = newTimeQuantum;
                result.timeQuantumUsed += 1;

                currentTimeQuantum = newTimeQuantum;
            }

            // Process yet to be done, evaluate
            if (readyQueue[i].remainingTime > 0.0f) {
                // Find the matching process to determine if it was the first time this process was ran.
                // If so, calculate the response time.
                for (j = 0; j < processesSize; ++j){
                    if (strcmp(readyQueue[i].processName, processes[j].name) == 0){
                        if (result.processResults[j].responseTime < 0.0f) {
                            result.processResults[j].responseTime = result.totalTime - processes[j].arrivalTime;
                            if (result.processResults[j].responseTime < 0.0f) { result.processResults[j].responseTime = 0.0f; }
                        }
                    }
                }

                processExecutedFlag = 1;

                // The remaining time left on process is more than the timeQuantum given...
                if (readyQueue[i].remainingTime > currentTimeQuantum) {
                    // Execute process till time quantum is up.
                    result.totalTime += currentTimeQuantum;
                    readyQueue[i].remainingTime -= currentTimeQuantum;
                }

                // If the remaining time on the current process fits on the quantumTime,
                // we can finish it now.
                if (readyQueue[i].remainingTime <= currentTimeQuantum) {
                    result.totalTime += readyQueue[i].remainingTime;
                    // Find the matching process to update the results of the finished process.
                    for (j = 0; j < processesSize; ++j){
                        if (strcmp(readyQueue[i].processName, processes[j].name) == 0){
                            result.processResults[j].waitingTime = result.totalTime - processes[j].arrivalTime - processes[j].burstTime;
                            result.processResults[j].turnaroundTime = result.processResults[j].waitingTime + processes[j].burstTime;
                        }
                    }
                    readyQueue[i].remainingTime = 0;
                }

                // This process was not the same as last, context switch occured.
                if (strcmp(lastProcess, readyQueue[i].processName) != 0) {
                    result.contextSwitch += 1;
                    strcpy(lastProcess, readyQueue[i].processName);
                }
            }


            int newProcessReadyFlag = 0;
            // Check if any processes are waiting to be added to the ready queue.
            for (j = 0; j < processesSize; ++j){
                if (addedProcessFlag[j] == 1){
                    // This process was already added to ready queue, ignore.
                    continue;
                }

                // A process has just arrived,
                // signal to add a process to the ready queue.
                if (processes[j].arrivalTime <= result.totalTime){
                    newProcessReadyFlag = 1;
                }
            }

            // New process waiting to be added, break out of loop to add them.
            if (newProcessReadyFlag == 1){
                break;
            }
        }

        int newProcessArrivedFlag = 0;
        int allProcessesDone = 1;
        // Loop through and add all arrived processes to the ready queue.
        for (j = 0; j < processesSize; ++j){
            if (addedProcessFlag[j] == 1){
                // This process was already added to ready queue, ignore.
                continue;
            }
            allProcessesDone = 0;

            // This process has arrive, add to  the ready queue.
            // Set signal to sort the queue later.
            if (processes[j].arrivalTime <= result.totalTime){
                strcpy(readyQueue[readyQueueSize].processName, processes[j].name);
                readyQueue[readyQueueSize].burstTime = processes[j].burstTime;
                readyQueue[readyQueueSize].remainingTime = processes[j].burstTime;
                ++readyQueueSize;

                newProcessArrivedFlag = 1;
                addedProcessFlag[j] = 1;
            }
        }

        // New process was added, sort the ready queue again.
        if (newProcessArrivedFlag == 1){
            qsort(readyQueue, readyQueueSize, sizeof(ReadyProcess), compare_ready_processes);
        }

        // If there are no new processes that arrived, but not all processes are done,
        // check if we need to skip time until the next arriving process.
        else if (newProcessArrivedFlag == 0 && allProcessesDone == 0){
            int skipTimeRequiredFlag = 1;
            for (i = 0; i < readyQueueSize; ++i){
                if (readyQueue[i].remainingTime > 0.0f){
                    skipTimeRequiredFlag = 0;
                    break;
                }
            }

            // Need to skip time until the next arriving process
            if (skipTimeRequiredFlag == 1){
                // Find the next earliest arrival
                float earliestArrival = -1.0f;
                for (j = 0; j < processesSize; ++j){
                    if (addedProcessFlag[j] == 1){
                        // This process was already added to ready queue, ignore.
                        continue;
                    }

                    if (earliestArrival < 0.0f || processes[j].arrivalTime < earliestArrival) {
                        earliestArrival = processes[j].arrivalTime;
                    }
                }

                // Skip time until earliest arrival.
                result.totalTime = earliestArrival;
            }
        }

        // All processes are done, just exit.
        else if (allProcessesDone == 1){
            break;
        }
    }

    result.avgWaitingTime = 0.0f;
    result.avgTurnaroundTime = 0.0f;
    result.avgResponseTime = 0.0f;

    for (int i = 0; i < processesSize; i++) {
        result.avgWaitingTime += result.processResults[i].waitingTime;
        result.avgTurnaroundTime += result.processResults[i].turnaroundTime;
        result.avgResponseTime += result.processResults[i].responseTime;
    }

    result.avgWaitingTime /= processesSize;
    result.avgTurnaroundTime /= processesSize;
    result.avgResponseTime /= processesSize;

    free(readyQueue);
    strcpy(result.roundRobinUsed, "Eighty Five Percentile Round Robin");
    return result;
}
