// CPU Scheduling Method : Round Robin
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"

int compare_processes(const void *a, const void *b) {
  Process *p1 = (Process *)a;
  Process *p2 = (Process *)b;

  return p1->burstTime - p2->burstTime;
}

float get_mean_burst_times(Process *processes, int processesSize) {
  float sum = 0.0f;
  for (int i = 0; i < processesSize; i++) {
    sum += processes[i].burstTime;
  }

  return sum / processesSize;
}

float get_median_burst_times(Process *processes, int processesSize) {
  qsort(processes, processesSize, sizeof(Process), compare_processes);

  if (processesSize % 2 == 0) {
    return (processes[processesSize / 2].burstTime + processes[(processesSize / 2) - 1].burstTime) / 2.0f;
  } else {
    return processes[processesSize / 2].burstTime;
  }
}

float get_lowest_arrival_time(Process *processes, int processesSize) {
  float lowestArrivalTime = FLT_MAX;
  int i;
  for (i = 0; i < processesSize; i++) {
    if (processes[i].arrivalTime < lowestArrivalTime) {
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
float find_next_arrival_time(float* remainingTime,Process* processes, int processesSize){
  float lowestArrivalTime = FLT_MAX;
  for (int i = 0; i < processesSize; ++i){
    if (remainingTime[i] <= 0.0f){
      continue;
    }

    if (lowestArrivalTime > processes[i].arrivalTime){
      lowestArrivalTime = processes[i].arrivalTime;
    }
  }
  return lowestArrivalTime;
}

RoundRobinResult round_robin(Process *processes, int processesSize, float timeQuantum, char* grouping) {
  int i;
  float remainingTime[processesSize];

  RoundRobinResult result;
  strcpy(result.grouping, grouping);
  result.processResults = (ProcessResult*) malloc(processesSize * sizeof(ProcessResult));
  result.processesSize = processesSize;

  // We start the lowest arrival time, simulate idling until a process arrives.
  result.totalTime = get_lowest_arrival_time(processes, processesSize);
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

  char lastProcess[MAX_NAME_LEN] = "";

  // This loop simulates the Round Robin on all of the processes.
  while (1) {
    int allProcessesDoneFlag = 1;
    for (i = 0; i < processesSize; i++) {
      // Ensure the response time for this process is never negative.
      result.processResults[i].responseTime = ((i  * timeQuantum) - processes[i].arrivalTime >= 0) ? ((i  * timeQuantum) - processes[i].arrivalTime) : 0;

      // Process has yet to arrive...
      if (processes[i].arrivalTime > result.totalTime){
        allProcessesDoneFlag = 0;
        continue;
      }

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
          result.processResults[i].turnaroundTime = result.processResults[i].waitingTime + processes[i].burstTime;
          remainingTime[i] = 0;
        }

        // This process was not the same as last, context switch occured.
        if (strcmp(lastProcess, processes[i].name) != 0){
          result.contextSwitch += 1;
          strcpy(lastProcess, processes[i].name);
        }
      }
    }

    // Not all processes are done yet
    float nextArrivalTime = find_next_arrival_time(remainingTime, processes, processesSize);
    if (nextArrivalTime != FLT_MAX && result.totalTime < nextArrivalTime){
      // The CPU now has to wait until the next process arrives, let the CPU wait.
      result.totalTime += (nextArrivalTime - result.totalTime);
      allProcessesDoneFlag = 0;
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
