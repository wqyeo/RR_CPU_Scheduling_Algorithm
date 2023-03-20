#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "../Models/ready_process.h"

int areEqual(float a, float b, float epsilon);
int compare_ready_processes(const void *a, const void *b);
int compare_processes(const void *a, const void *b);
float get_mean_burst_times(Process *processes, int processesSize);
float get_median_burst_times(Process *processes, int processesSize);
// Find the lowest arrival time from all processes.
float get_lowest_arrival_time(Process *processes, int processesSize);
// Find the lowest arrival time from the undone process.
float find_next_arrival_time(float* remainingTime,Process* processes, int processesSize);

int ready_queue_done(ReadyProcess* readyQueue, int readyQueueSize);
int all_processes_added(int* addedProcessFlag, int processesSize);

RoundRobinResult round_robin(Process *processes, int processesSize, float timeQuantum, char* grouping);

#endif
