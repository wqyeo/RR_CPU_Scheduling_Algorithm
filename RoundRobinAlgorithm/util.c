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

void append_main_result(FILE* filePointer, RoundRobinResult result){
	if (filePointer == NULL){return;}

	fprintf(filePointer, "[%s]\n", result.roundRobinUsed);
	fprintf(filePointer, "time_quantum=%f\n", result.timeQuantum);

	fprintf(filePointer, "average_waiting_time=%f\n", result.avgWaitingTime);
	fprintf(filePointer, "average_turnaround_time=%f\n", result.avgTurnaroundTime);
	fprintf(filePointer, "average_response_time=%f\n", result.avgResponseTime);

	fprintf(filePointer, "total_time_taken=%f\n", result.totalTime);
	fprintf(filePointer, "context_switch_count=%d\n", result.contextSwitch);

	fprintf(filePointer, "total_processes_count=%d\n",result.processesSize);
}

void append_process_result(FILE* filePointer, ProcessResult processResult){
	if (filePointer == NULL){return;}

	fprintf(filePointer, "(%s)\n", processResult.process.name);
	fprintf(filePointer, "arrival_time=%f\n", processResult.process.arrivalTime);
	fprintf(filePointer, "burst_time=%f\n", processResult.process.burstTime);


	fprintf(filePointer, "waiting_time=%f\n", processResult.waitingTime);
	fprintf(filePointer, "response_time=%f\n", processResult.responseTime);
	fprintf(filePointer, "turnaround_time=%f\n", processResult.turnaroundTime);
}

void save_result_to_file(char* fileName, RoundRobinResult result) {
    FILE *filePointer;
	char extensionName[] = ".dat";
	char filePath[512] = "";

	strcat(filePath, fileName);
	strcat(filePath, extensionName);
    filePointer = fopen(filePath, "w+");

    if (filePointer == NULL) {
        printf("\nError opening file.");
        return;
    }

	append_main_result(filePointer, result);

	int i;
	for (i = 0; i < result.processesSize; ++i){
		ProcessResult processResult = result.processResults[i];
		append_process_result(filePointer, processResult);
	}

    fclose(filePointer);
}
