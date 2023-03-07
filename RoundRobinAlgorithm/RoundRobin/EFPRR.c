// CPU Scheduling Method : EIGHTY FIVE PERCENTILE ROUND ROBIN CPU SCHEDULING ALGORITHM
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "../Models/process.h"
#include "../Models/process_result.h"
#include "../Models/round_robin_result.h"

#include "round_robin.h"

float calc_active_burst_average(Process *processes, float *remainingTime, int processesSize, float currentTime){
    printf("============\n");
    float numOfRemaining = 0;
    float sumOfProcessesBurst = 0;
    for(int i = 0 ; i < processesSize ; i++){
        if(remainingTime[i] > 0.0f && currentTime >= processes[i].arrivalTime){
            printf("%s", processes[i].name);
            numOfRemaining += 1.0f;
            sumOfProcessesBurst += processes[i].burstTime;
        }
    }
    return sumOfProcessesBurst / numOfRemaining;
}

void merge(Process* arr, int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  Process L[n1], R[n2];

  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  i = 0;
  j = 0;
  k = left;

  while (i < n1 && j < n2) {
    if (L[i].arrivalTime < R[j].arrivalTime) {
      arr[k] = L[i];
      i++;
    } else if (L[i].arrivalTime == R[j].arrivalTime && L[i].burstTime < R[j].burstTime) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(Process* arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

RoundRobinResult EFPRR(Process *processes, int processesSize, char* grouping) {
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

  char lastProcess[MAX_NAME_LEN] = "";
  // Same as simulating basic Round Robin here, except with a few changes due to the modified algorithm...
  mergeSort(processes, 0, processesSize - 1);
  while (1) {
    result.timeQuantum = calc_active_burst_average(processes, remainingTime, processesSize, result.totalTime) * 0.85f;
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
        printf("%s, %.2f ", processes[i].name, result.timeQuantum);
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

  strcpy(result.roundRobinUsed, "Eighty Five Percentile Round Robin");
  return result;
}
