#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round_robin_result.h"

#include "round_robin.h"
#include "manhatten_round_robin.h"
#include "best_quantum_time_round_robin.h"

void print_round_robin_result(RoundRobinResult result){
    int i;
    printf("\nCPU Scheduling Algorithm (%s)\n", result.roundRobinUsed);
    printf("\nTime Quantum: %.2f", result.timeQuantum);

    // Print out a table of the times for each of the process.
    printf("\nPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (i = 0; i < result.processesSize; i++) {
        printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
               result.processResults[i].process.name, result.processResults[i].process.burstTime, result.processResults[i].waitingTime, result.processResults[i].turnaroundTime, result.processResults[i].responseTime);
    }

    printf("\nAverage Waiting Time: %.2f\n", result.avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", result.avgTurnaroundTime);
    printf("Average Response Time: %.2f\n", result.avgResponseTime);
    printf("Context Switch: %d\n", result.contextSwitch);
    printf("Total Time Taken: %.2f\n", result.totalTime);
}

int request_processes_count(){
    int processesCount = 0;
    printf("\n\tInput the number of processes: ");
    scanf("%d", &processesCount);
    return processesCount;
}

Process *request_processes(int size) {
  Process *processes = malloc(size * sizeof(Process));

  int i;
  for (i = 0; i < size; i++) {
    printf("\nEnter name for process %d (max %d characters): ", i, MAX_NAME_LEN-1);
    scanf("%s", processes[i].name);
    printf("Enter burst time for process %s: ", processes[i].name);
    scanf("%f", &processes[i].burstTime);
    printf("Enter arrival time for process %s: ", processes[i].name);
    scanf("%f", &processes[i].arrivalTime);
  }

  return processes;
}

void perform_round_robin(Process *processes, int processesCount){
    float timeQuantum;
    printf("\n\tInput time quantum: ");
    scanf("%f", &timeQuantum);

    round_robin(processes, processesCount, timeQuantum);
}

int main() {
    int processesCount = request_processes_count();
    Process *processes = request_processes(processesCount);

    RoundRobinResult result = modified_round_robin(processes, processesCount);
    print_round_robin_result(result);

    free(processes);
    free(result.processResults);
    return 0;
}
