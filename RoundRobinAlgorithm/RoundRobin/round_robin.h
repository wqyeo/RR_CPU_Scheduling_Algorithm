#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

int compare_processes(const void *a, const void *b);
float get_mean_burst_times(Process *processes, int processesSize);
float get_median_burst_times(Process *processes, int processesSize);
// Find the lowest arrival time from all processes.
int get_lowest_arrival_time(Process *processes, int processesSize);
// Find the lowest arrival time from the undone process.
int find_next_arrival_time(float* remainingTime,Process* processes, int processesSize);

RoundRobinResult round_robin(Process *processes, int processesSize, float timeQuantum);

#endif
