#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round_robin.h"

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

void perform_round_robin(){
    int processesCount = request_processes_count();
    Process *processes = request_processes(processesCount);

    float timeQuantum;
    printf("\n\tInput time quantum: ");
    scanf("%f", &timeQuantum);

    round_robin(processes, processesCount, timeQuantum);
    free(processes);
}

int main() {
    perform_round_robin();
    return 0;
}
