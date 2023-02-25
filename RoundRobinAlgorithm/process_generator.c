#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "Models/process.h"
#include "Models/process_result.h"
#include "Models/round_robin_result.h"

#define MAX_BURST_TIME 54
#define MIN_BURST_TIME 1

#define MAX_ARRIVAL_TIME 20

Process* generate_random_processes(int processesSize){
	Process* processes = (Process*) malloc(processesSize * sizeof(Process));
	srand(time(NULL));
	int i;
	for (i = 0; i < processesSize; ++i){
		char processName[MAX_NAME_LEN];
		snprintf(processName, MAX_NAME_LEN, "P%d", i);
		strcpy(processes[i].name, processName);

		float generatedBurstTime = ((float) rand() / RAND_MAX) * (MAX_BURST_TIME - MIN_BURST_TIME) + MIN_BURST_TIME;
		float generatedArrivalTime = ((float) rand() / RAND_MAX) * (MAX_ARRIVAL_TIME);
		processes[i].burstTime = generatedBurstTime;
		processes[i].arrivalTime = generatedArrivalTime;
	}
	return processes;
}

