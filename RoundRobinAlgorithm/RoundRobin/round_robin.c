// CPU Scheduling Method : Round Robin
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <math.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"
#include "../Models/ready_process.h"



/**
* For comparing between floats
*
* https://stackoverflow.com/questions/5989191/compare-two-floats
* TLDR; Because floats are not precise.
*/
int areEqual(float a, float b, float epsilon) {
    return fabs(a - b) < epsilon;
}

/**
* Used for sorting processes by lowest burst time to highest.
*/
int compare_processes(const void *a, const void *b)
{
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    return p1->burstTime - p2->burstTime;
}

/**
* Same as 'compare_processes', but for ReadyProcess struct types.
*/
int compare_ready_processes(const void *a, const void *b)
{
    ReadyProcess *p1 = (ReadyProcess *)a;
    ReadyProcess *p2 = (ReadyProcess *)b;

    return p1->burstTime - p2->burstTime;
}

float get_mean_burst_times(Process *processes, int processesSize)
{
    float sum = 0.0f;
    for (int i = 0; i < processesSize; i++)
    {
        sum += processes[i].burstTime;
    }

    return sum / processesSize;
}

float get_median_burst_times(Process *processes, int processesSize)
{
    qsort(processes, processesSize, sizeof(Process), compare_processes);

    if (processesSize % 2 == 0)
    {
        return (processes[processesSize / 2].burstTime + processes[(processesSize / 2) - 1].burstTime) / 2.0f;
    }
    else
    {
        return processes[processesSize / 2].burstTime;
    }
}

float get_lowest_arrival_time(Process *processes, int processesSize)
{
    float lowestArrivalTime = FLT_MAX;
    int i;
    for (i = 0; i < processesSize; i++)
    {
        if (processes[i].arrivalTime < lowestArrivalTime)
        {
            lowestArrivalTime = processes[i].arrivalTime;
        }
    }
    return lowestArrivalTime;
}

/**
 * Same as get_lowest_arrival_time, but this function considers if the process is done or not.
 *
 * @param remainingTime The array indicating the time remaining to complete the processes.
 * @param processes The array of processes.
 * @param processesSize Number of processes.
 * @return The lowest arrival time of an undone process. FLT_MAX if none.
 */
float find_next_arrival_time(float* remainingTime,Process* processes, int processesSize)
{
    float lowestArrivalTime = FLT_MAX;
    for (int i = 0; i < processesSize; ++i)
    {
        if (remainingTime[i] <= 0.0f)
        {
            continue;
        }

        if (lowestArrivalTime > processes[i].arrivalTime)
        {
            lowestArrivalTime = processes[i].arrivalTime;
        }
    }
    return lowestArrivalTime;
}

/**
* Helper function to check if all processes in a ready queue is done or not.
* @param readyQueue The ready queue itself.
* @param readyQueueSize The current size of the ready queue.
*/
int ready_queue_done(ReadyProcess* readyQueue, int readyQueueSize){
    int allProcessesDone = 1;

    int i;
    for (i = 0; i < readyQueueSize; i++)
    {
        if (readyQueue[i].remainingTime > 0.0f) {
            allProcessesDone = 0;
            break;
        }
    }
    return allProcessesDone;
}

/**
* Helper function to check if all processes are added.
* @param addedProcessFlag The list of flags representing if an indexed process has been added.
* @param processesSize The size for the list of all processes.
*/
int all_processes_added(int* addedProcessFlag, int processesSize){
    int allProcessesAdded = 1;

    int i;
    for (i = 0; i < processesSize; ++i)
    {
        // This process was not added yet, hence not done.
        if (addedProcessFlag[i] == 0)
        {
            allProcessesAdded = 0;
            continue;
        }
    }
    return allProcessesAdded;
}

/**
* @param grouping This is used to group same tests cases together; For analysis scripts.
*/
RoundRobinResult round_robin(Process *processes, int processesSize, float timeQuantum, char* grouping)
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

    float currentTimeQuantum = timeQuantum;
    result.timeQuantums[0] = timeQuantum;
    result.timeQuantumUsed = 1;
    while (1)
    {

        for (i = 0; i < readyQueueSize; i++)
        {
            // Process yet to be done, evaluate
            if (readyQueue[i].remainingTime > 0.0f)
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
                // Else if the remaining time on the current process fits on the quantumTime,
                // we can finish it now.
                else if (readyQueue[i].remainingTime <= currentTimeQuantum)
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
                }
            }
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
    strcpy(result.roundRobinUsed, "Round Robin");

    return result;
}
