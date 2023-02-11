#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round_robin.h"

int request_processes_count(){
    int processes_count = 0;
    printf("\n\tInput the number of processes: ");
    scanf("%d", &processes_count);
    return processes_count;
}

Process *request_processes(int size) {
  Process *processes = malloc(size * sizeof(Process));

  int i;
  for (i = 0; i < size; i++) {
    printf("\nEnter name for process %d (max %d characters): ", i, MAX_NAME_LEN-1);
    scanf("%s", processes[i].name);
    printf("Enter burst time for process %s: ", processes[i].name);
    scanf("%f", &processes[i].burst_time);
    printf("Enter arrival time for process %s: ", processes[i].name);
    scanf("%f", &processes[i].arrival_time);
  }

  return processes;
}

int main() {
    int processes_count = request_processes_count();
    Process *processes = request_processes(numProcesses);

    round_robin(processes, processes_count);

    free(processes);
    return 0;
}
