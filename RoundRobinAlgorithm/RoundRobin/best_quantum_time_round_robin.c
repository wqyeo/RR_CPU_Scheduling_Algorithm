// CPU Scheduling Method : BEST TIME QUANTUM ROUND ROBIN CPU SCHEDULING ALGORITHM
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"

#include "round_robin.h"

RoundRobinResult modified_round_robin(Process *processes, int processesSize, char* grouping) {
  RoundRobinResult result;
  int i;
  float remainingTime[processesSize];

  strcpy(result.grouping, grouping);
  result.processesSize = processesSize;
  result.processResults = (ProcessResult *)malloc(sizeof(ProcessResult) * processesSize);
  // Start at the lowest arrival time, simulate idling until first process comes.
  result.totalTime = get_lowest_arrival_time(processes, processesSize);
  result.contextSwitch = 0;

  // Sort from shortest to longest burst time.
  qsort(processes, processesSize, sizeof(Process), compare_processes);

  // init
  for (i = 0; i < processesSize; i++) {
    remainingTime[i] = processes[i].burstTime;
    result.processResults[i].waitingTime = 0.0f;
    result.processResults[i].turnaroundTime = 0.0f;
    result.processResults[i].responseTime = 0.0f;
    result.processResults[i].process = processes[i];
  }

  // TimeQuantum is now calculated as such:
  float mean = get_mean_burst_times(processes, processesSize);
  float median = get_median_burst_times(processes, processesSize);
  result.timeQuantum = (mean + median) / 2.0f;

  char lastProcess[MAX_NAME_LEN] = "";
  // Same as simulating basic Round Robin here, except with a few changes due to the modified algorithm...
  while (1) {
    int allProcessesDoneFlag = 1;
    for (i = 0; i < processesSize; i++) {
      result.processResults[i].responseTime = ((i  * result.timeQuantum) - processes[i].arrivalTime >= 0) ? ((i  * result.timeQuantum) - processes[i].arrivalTime) : 0;

      // Process has yet to arrive, ignore.
      if (processes[i].arrivalTime > result.totalTime){
        allProcessesDoneFlag = 0;
        continue;
      }

      // Process yet to be done, evaluate
      if (remainingTime[i] > 0) {
        allProcessesDoneFlag = 0;

        // The remaining time left on process is more than the timeQuantum given...
        if (remainingTime[i] > result.timeQuantum) {
          // Execute process till time quantum is up.
          result.totalTime += result.timeQuantum;
          remainingTime[i] -= result.timeQuantum;
        }

        // If the remaining time on the current process fits on the quantumTime,
        // we can finish it now.
        // (Modified round robin ensures that this check occurs after executing the process.)
        if (remainingTime[i] <= result.timeQuantum) {
          // Execute process till end, then calculate the times for this process.
          result.totalTime += remainingTime[i];
          result.processResults[i].waitingTime = result.totalTime - processes[i].arrivalTime - processes[i].burstTime;
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
    if (nextArrivalTime != INT_MAX && result.totalTime < nextArrivalTime){
      // The CPU now has to wait until the next process arrives, let the CPU wait.
      result.totalTime += (nextArrivalTime - result.totalTime);
      allProcessesDoneFlag = 0;
    }

    if (allProcessesDoneFlag == 1) {
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

  strcpy(result.roundRobinUsed, "Best Quantum Time Round Robin");
  return result;
}
