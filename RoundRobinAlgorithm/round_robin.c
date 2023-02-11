// CPU Scheduling Method : Round Robin
#include <stdio.h>
#include "process.h"

void round_robin(Process *processes, int processes_size) {
    int i, j;
    float current_time = 0;
    float remaining_time[processes_size];
    float waiting_time[processes_size];
    float turnaround_time[processes_size];
    float response_time[processes_size];

    for (i = 0; i < processes_size; i++) {
        remaining_time[i] = (int) processes[i].burst_time;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
        response_time[i] = 0;
    }

    float time_quantum;
    printf("\n\tInput time quantum: ");
    scanf("%f", &time_quantum);

    // This loop simulates the Round Robin on all of the processes.
    while (1) {
        int all_processes_done_flag = 1;
        for (i = 0; i < processes_size; i++) {
            // Ensure the response time for this process is never negative.
            response_time[i] = ((i  * time_quantum) - processes[i].arrival_time >= 0) ? ((i  * time_quantum) - processes[i].arrival_time) : 0;

            // Unfinished process...
            if (remaining_time[i] > 0) {
                all_processes_done_flag = 0;
                if (remaining_time[i] > time_quantum) {
                    // Process won't finish by this time quantum.
                    // Increment current time by the time quantum, then decrease the process's remaining time.
                    current_time += time_quantum;
                    remaining_time[i] -= time_quantum;
                } else {
                    // Process will finish by this time quantum, increment current time by remaining time.
                    // Then, calculate the times for this process...
                    current_time += remaining_time[i];
                    waiting_time[i] = current_time - processes[i].arrival_time - processes[i].burst_time;
                    turnaround_time[i] = current_time - processes[i].arrival_time;
                    remaining_time[i] = 0;
                }
            }
        }
        if (all_processes_done_flag == 1) {
            break;
        }
    }

    // Print out a table of the times for each of the process.
    printf("\nPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (i = 0; i < processes_size; i++) {
        printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", processes[i].name, processes[i].burst_time, waiting_time[i], turnaround_time[i], response_time[i]);
    }

    // Calculate average of times, then print out.
    float avg_waiting_time = 0.0, avg_turnaround_time = 0.0, avg_response_time = 0.0;
    for (i = 0; i < processes_size; i++) {
        avg_waiting_time += waiting_time[i];
        avg_turnaround_time += turnaround_time[i];
        avg_response_time += response_time[i];
    }
    avg_waiting_time /= processes_size;
    avg_turnaround_time /= processes_size;
    avg_response_time /= processes_size;

    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
}
