// CPU Scheduling Mehod : BEST TIME QUANTUM ROUND ROBIN CPU SCHEDULING ALGORITHM
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"
#include "../Models/ready_process.h"

#include "round_robin.h"

/**
* Find the median of burst times in the ready queue.
*/
float median_ready_queue(ReadyProcess* readyQueue, int readyQueueSize)
{
    ReadyProcess undoneProcesses[readyQueueSize];
    int undoneProcessesSize = 0;

    // Filter out all processes that are done
    for (int i=0; i<readyQueueSize; i++)
    {
        if (readyQueue[i].remainingTime > 0.0f)
        {
            undoneProcesses[undoneProcessesSize] = readyQueue[i];
            undoneProcessesSize++;
        }
    }

    for (int i=0; i < undoneProcessesSize-1; i++)
    {
        for (int j=i+1; j<undoneProcessesSize; j++)
        {
            if (undoneProcesses[i].remainingTime > undoneProcesses[j].remainingTime)
            {
                ReadyProcess temp = undoneProcesses[i];
                undoneProcesses[i] = undoneProcesses[j];
                undoneProcesses[j] = temp;
            }
        }
    }

    int median;
    if (undoneProcessesSize % 2 == 0)
    {
        median = (undoneProcesses[undoneProcessesSize/2].remainingTime + undoneProcesses[(undoneProcessesSize/2)-1].remainingTime) / 2;
    }
    else
    {
        median = undoneProcesses[undoneProcessesSize/2].remainingTime;
    }

    return median;
}

float mean_ready_queue(ReadyProcess* readyQueue, int readyQueueSize)
{
    float sum = 0;
    int count = 0;

    for (int i = 0; i < readyQueueSize; i++)
    {
        // Process is not done, count.
        if (readyQueue[i].remainingTime > 0.0f)
        {
            sum += readyQueue[i].remainingTime;
            count++;
        }
    }

    // Return mean
    return (sum / count);
}

float find_best_time_quantum(ReadyProcess* readyQueue, int readyQueueSize){
   float mean = mean_ready_queue(readyQueue, readyQueueSize);
  float median = median_ready_queue(readyQueue, readyQueueSize);

  return (mean + median) / 2.0f;
}

RoundRobinResult modified_round_robin(Process *processes, int processesSize, char* grouping) {
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

    for (i = 0; i < processesSize; ++i)
    {
        result.processResults[i].waitingTime = 0.0f;
        result.processResults[i].turnaroundTime = 0.0f;
        result.processResults[i].responseTime = -1.0f;
        result.processResults[i].process = processes[i];

        // Process arrives at the start, add to ready queue.
        if (processes[i].arrivalTime <= result.totalTime)
        {
            strcpy(readyQueue[readyQueueSize].processName, processes[i].name);
            readyQueue[readyQueueSize].remainingTime = processes[i].burstTime;
            readyQueue[readyQueueSize].burstTime = processes[i].burstTime;
            ++readyQueueSize;

            // Mark the position of the process has been added.
            addedProcessFlag[i] = 1;
        }
        else
        {
            addedProcessFlag[i] = 0;
        }
    }

    char lastProcess[MAX_NAME_LEN] = "";

    qsort(readyQueue, readyQueueSize, sizeof(ReadyProcess), compare_ready_processes);
    float currentTimeQuantum = find_best_time_quantum(readyQueue, readyQueueSize);
    result.timeQuantums[0] = currentTimeQuantum;
    result.timeQuantumUsed = 1;

    int timeSkipOccuredFlag = 0;
    while (1)
    {
        // This flag is set to true if a new process arrives as the CPU is executing the ready queue.
        // (Used to signal calculation of Time Quantum at the end of the ready queue.)
        int processHadArrivedFlag = 0;
        for (i = 0; i < readyQueueSize; i++)
        {
            // Process yet to be done, evaluate.
            //
            // Additionally, if time skip occured, then dont execute anything.
            // (This is because if time skip had occured, the CPU was just idling/waiting for a process to arrive,
            // hence, we ignore executing anything and just add all arrived process into the ready queue.
            // then, we calculate manhattan time quantum again based on ready queue.)
            if (readyQueue[i].remainingTime > 0.0f && timeSkipOccuredFlag == 0)
            {
                // Find the matching process to determine if it was the first time this process was ran.
                // If so, calculate the response time.
                for (j = 0; j < processesSize; ++j)
                {
                    if (strcmp(readyQueue[i].processName, processes[j].name) == 0)
                    {
                        if (result.processResults[j].responseTime < 0.0f)
                        {
                            result.processResults[j].responseTime = result.totalTime - processes[j].arrivalTime;
                            if (result.processResults[j].responseTime < 0.0f)
                            {
                                result.processResults[j].responseTime = 0.0f;
                            }
                        }
                    }
                }

                // The remaining time left on process is more than the timeQuantum given...
                if (readyQueue[i].remainingTime > currentTimeQuantum)
                {
                    // Execute process till time quantum is up.
                    result.totalTime += currentTimeQuantum;
                    readyQueue[i].remainingTime -= currentTimeQuantum;
                }

                // If the remaining time on the current process fits on the quantumTime,
                // we can finish it now.
                // NOTE:  Modified round robin ensures this check can occur after
                if (readyQueue[i].remainingTime <= currentTimeQuantum)
                {
                    result.totalTime += readyQueue[i].remainingTime;
                    // This loop finds the matching process on the result lists,
                    // to update the results.
                    for (j = 0; j < processesSize; ++j)
                    {
                        if (strcmp(readyQueue[i].processName, processes[j].name) == 0)
                        {
                            result.processResults[j].waitingTime = result.totalTime - processes[j].arrivalTime - processes[j].burstTime;
                            result.processResults[j].turnaroundTime = result.processResults[j].waitingTime + processes[j].burstTime;
                        }
                    }
                    readyQueue[i].remainingTime = 0;
                }

                // This process was not the same as last, context switch occured.
                if (strcmp(lastProcess, readyQueue[i].processName) != 0)
                {
                    result.contextSwitch += 1;
                    strcpy(lastProcess, readyQueue[i].processName);
                }
            }

            // Check if any processes are waiting to be added to the ready queue.
            for (j = 0; j < processesSize; ++j)
            {
                if (addedProcessFlag[j] == 1)
                {
                    // This process was already added to ready queue, ignore.
                    continue;
                }

                // A process has just arrived,
                // Add to the end of the ready queue.
                if (processes[j].arrivalTime <= result.totalTime)
                {
                    strcpy(readyQueue[readyQueueSize].processName, processes[j].name);
                    readyQueue[readyQueueSize].burstTime = processes[j].burstTime;
                    readyQueue[readyQueueSize].remainingTime = processes[j].burstTime;
                    ++readyQueueSize;
                    addedProcessFlag[j] = 1;
                    processHadArrivedFlag = 1;
                }
            }
        }

        timeSkipOccuredFlag = 0;
        if (processHadArrivedFlag == 1){
            float newTimeQuantum = find_best_time_quantum(readyQueue, readyQueueSize);
            // If this is a new time quantum, record it.
            if (areEqual(newTimeQuantum, currentTimeQuantum, 0.0001f) == 0){
                currentTimeQuantum = newTimeQuantum;
                result.timeQuantums[result.timeQuantumUsed] = newTimeQuantum;
                result.timeQuantumUsed += 1;
            }
            // Additionally, sort ready queue since it was modified.
            qsort(readyQueue, readyQueueSize, sizeof(ReadyProcess), compare_ready_processes);
        }

        // Check if processes are done or added.
        int allProcessesDone = ready_queue_done(readyQueue, readyQueueSize);
        if (all_processes_added(addedProcessFlag, allProcessesDone) == 0){
            allProcessesDone = 0;
        }

        // If not all processes are done,
        // check if we need to skip time until the next arriving process.
        if (allProcessesDone == 0)
        {
            int skipTimeRequiredFlag = 1;
            for (i = 0; i < readyQueueSize; ++i)
            {
                if (readyQueue[i].remainingTime > 0.0f)
                {
                    skipTimeRequiredFlag = 0;
                    break;
                }
            }

            // Need to skip time until the next arriving process
            if (skipTimeRequiredFlag == 1)
            {
                // Find the next earliest arrival
                float earliestArrival = -1.0f;
                for (j = 0; j < processesSize; ++j)
                {
                    if (addedProcessFlag[j] == 1)
                    {
                        // This process was already added to ready queue, ignore.
                        continue;
                    }

                    if (earliestArrival < 0.0f || processes[j].arrivalTime < earliestArrival)
                    {
                        earliestArrival = processes[j].arrivalTime;
                    }
                }

                // Skip time until earliest arrival.
                result.totalTime = earliestArrival;
                timeSkipOccuredFlag = 1;
            }
        }

        // All processes are done, just exit.
        else if (allProcessesDone == 1)
        {
            break;
        }
    }

    result.avgWaitingTime = 0.0f;
    result.avgTurnaroundTime = 0.0f;
    result.avgResponseTime = 0.0f;

    for (int i = 0; i < processesSize; i++)
    {
        result.avgWaitingTime += result.processResults[i].waitingTime;
        result.avgTurnaroundTime += result.processResults[i].turnaroundTime;
        result.avgResponseTime += result.processResults[i].responseTime;
    }

    result.avgWaitingTime /= processesSize;
    result.avgTurnaroundTime /= processesSize;
    result.avgResponseTime /= processesSize;

    free(readyQueue);
    strcpy(result.roundRobinUsed, "Best Time Quantum Round Robin");

    return result;
}
