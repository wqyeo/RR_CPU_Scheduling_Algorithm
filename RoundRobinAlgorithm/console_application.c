#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "console_application.h"
#include "process_generator.h"

#include "Models/process.h"
#include "Models/round_robin_result.h"

#include "RoundRobin/round_robin.h"
#include "RoundRobin/manhatten_round_robin.h"
#include "RoundRobin/best_quantum_time_round_robin.h"

#include "Util/color_print.h"
#include "Util/data_saver.h"

#define MAX_DATETIME_LEN 32

char *generate_random_string(int length) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *random_string = malloc(sizeof(char) * (length + 1));
    srand(time(NULL));
    
    for (int i = 0; i < length; i++) {
        int index = rand() % (int)(sizeof(charset) - 1);
        random_string[i] = charset[index];
    }
    
    random_string[length] = '\0';
    
    return random_string;
}

char* get_current_datetime_string() {
    char* datetime = malloc(MAX_DATETIME_LEN * sizeof(char));
    if (!datetime) {
        return NULL; // failed to allocate memory
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // format the date-time string
    strftime(datetime, MAX_DATETIME_LEN, "%Y-%m-%d %H:%M:%S", t);

    return datetime;
}

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

int request_processes_count() {
    int processesCount = 0;
    char input[MAX_USER_INPUT_SIZE];

    while (1) {
        PRINT_WHITE("\n\tInput the number of processes: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input, ensure its all numbers\n");
            continue;
        }
        if (sscanf(input, "%d", &processesCount) == 1) {
            // input is a valid number
            break;
        }
        PRINT_YELLOW("\tInvalid input, please enter a number\n");
    }

    return processesCount;
}

void clear_input_buffer(){
    while (getchar() != '\n');
}

Process *request_processes(int size) {
    Process *processes = malloc(size * sizeof(Process));

    int i;
    for (i = 0; i < size; i++) {
    printf("\nEnter name for process %d (max %d characters): ", i, MAX_NAME_LEN-1);
    scanf("%s", processes[i].name);

    // Re-request until valid input is entered for burst time
    while (1) {
      printf("Enter burst time for process %s: ", processes[i].name);
      if (scanf("%f", &processes[i].burstTime) == 1) {
        break;
      } else {
        printf("Invalid input. Please enter a valid number.\n");
        clear_input_buffer();
      }
    }

    // Re-request until valid input is entered for arrival time
    while (1) {
      printf("Enter arrival time for process %s: ", processes[i].name);
      if (scanf("%f", &processes[i].arrivalTime) == 1) {
        break;
      } else {
        printf("Invalid input. Please enter a valid number.\n");
        clear_input_buffer();
      }
    }
    }

    return processes;
}

RoundRobinMode request_round_robin_mode(){
    RoundRobinMode userInput;
    char inputStr[MAX_USER_INPUT_SIZE];

    while (1) {
        PRINT_WHITE("\nSelect Mode:\n");
        PRINT_GRAY("1 - All (Run all implemented round robin tests) \n");
        PRINT_GRAY("2 - Round Robin\n");
        PRINT_GRAY("3 - Manhatten Round Robin\n");
        PRINT_GRAY("4 - Best Quantum Time Round Robin\n");
        PRINT_WHITE("(1/2/3/4):\n");
        fgets(inputStr, MAX_USER_INPUT_SIZE, stdin);

        // Remove the 'enter' key
        inputStr[strcspn(inputStr, "\n")] = '\0';

        for (int i = 0; i < strlen(inputStr); i++) {
            inputStr[i] = tolower(inputStr[i]);
        }


        if (strcmp(inputStr, "1") == 0) {
            userInput = ALL;
            break;
        } else if (strcmp(inputStr, "2") == 0) {
            userInput = ROUND_ROBIN;
            break;
        } else if (strcmp(inputStr, "3") == 0) {
            userInput = MANHATTEN_ROUND_ROBIN;
            break;
        } else if (strcmp(inputStr, "4") == 0) {
            userInput = BEST_QUANTUM_TIME_ROUND_ROBIN;
            break;
        } else {
            PRINT_YELLOW("Invalid input. Please enter 1, 2, 3, 4.\n");
        }
    }
    return userInput;
}

RoundRobinResult perform_round_robin(Process *processes, int processesCount, char* groupingString){
    float timeQuantum;
    // Re-request until valid input is entered for burst time
    while (1) {
      printf("Enter Quantum Time for Round Robin ");
      if (scanf("%f", &timeQuantum) == 1) {
        break;
      } else {
        printf("Invalid input. Please enter a valid number.\n");
        clear_input_buffer();
      }
    }

    return round_robin(processes, processesCount, timeQuantum, groupingString);
}

// Deep clone
Process* clone_processes(Process* processes, int size) {
  Process* new_processes = malloc(size * sizeof(Process));
  int i;
  for (i = 0; i < size; i++) {
    // copy the name of the process
    strcpy(new_processes[i].name, processes[i].name);

    // copy the burst and arrival times
    new_processes[i].burstTime = processes[i].burstTime;
    new_processes[i].arrivalTime = processes[i].arrivalTime;
  }
  return new_processes;
}

void run_round_robin_tests(RunMode runMode){
    int processCount = request_processes_count();
    Process* processes;
    if (runMode == MANUAL) {
        processes = request_processes(processCount);
    } else {
        processes = generate_random_processes(processCount);
    }

    RoundRobinMode roundRobinToUse = request_round_robin_mode();
    char* groupingString = generate_random_string(62);
	if (roundRobinToUse == ALL){
        // Simulate Round Robin first
        Process* roundRobinProcesses = clone_processes(processes, processCount);
        RoundRobinResult roundRobinResult = perform_round_robin(roundRobinProcesses, processCount, groupingString);
        // Print and save
        print_round_robin_result(roundRobinResult);
        char fileName[] = "RoundRobin_";
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, roundRobinResult);
        // Free memory
        free(roundRobinProcesses);
        free(roundRobinResult.processResults);
        PRINT_BLUE("\n=================\n");

        Process* manhattanProcesses = clone_processes(processes, processCount);
        RoundRobinResult manhattanResult = manhattan_round_robin(manhattanProcesses, processCount, groupingString);
        print_round_robin_result(manhattanResult);
        strcpy(fileName, "Manhattan_");
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, manhattanResult);

        free(manhattanProcesses);
        free(manhattanResult.processResults);
        PRINT_BLUE("\n=================\n");

        Process* bestQuantumProcess = clone_processes(processes, processCount);
        RoundRobinResult modifiedResult = modified_round_robin(bestQuantumProcess, processCount, groupingString);
        print_round_robin_result(modifiedResult);
        strcpy(fileName, "BestTimeQuantum_");
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, modifiedResult);

        free(bestQuantumProcess);
        free(modifiedResult.processResults);
        PRINT_BLUE("\n=================\n");
    } else if (roundRobinToUse == ROUND_ROBIN){
        RoundRobinResult roundRobinResult = perform_round_robin(processes, processCount, groupingString);
        print_round_robin_result(roundRobinResult);
        char fileName[] = "RoundRobin_";
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, roundRobinResult);

        free(roundRobinResult.processResults);
    } else if (roundRobinToUse == MANHATTEN_ROUND_ROBIN){
        RoundRobinResult manhattanResult = manhattan_round_robin(processes, processCount, groupingString);
        print_round_robin_result(manhattanResult);
        char fileName[] = "Manhattan_";
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, manhattanResult);

        free(manhattanResult.processResults);
    } else if (roundRobinToUse == BEST_QUANTUM_TIME_ROUND_ROBIN){
        RoundRobinResult bestQuantumTimeResult = modified_round_robin(processes, processCount, groupingString);
        print_round_robin_result(bestQuantumTimeResult);
        char fileName[] = "BestQuantumTime_";
        strcat(fileName, get_current_datetime_string());
        save_result_to_file(fileName, bestQuantumTimeResult);

        free(bestQuantumTimeResult.processResults);
    }

    free(processes);
}

void run_application(){
    PRINT_WHITE("Round Robin Simulator...\n");
    char inputStr[MAX_USER_INPUT_SIZE];

    while (1) {
        PRINT_WHITE("\nSelect Mode:\n");
        PRINT_GRAY("1 (Manual) - Manually input processes.\n");
        PRINT_GRAY("2 (Auto) - Automatically generate processes.\n");
        PRINT_WHITE("(1/2/EXIT):\n");
        fgets(inputStr, MAX_USER_INPUT_SIZE, stdin);

        // Remove the 'enter' key
        inputStr[strcspn(inputStr, "\n")] = '\0';

        for (int i = 0; i < strlen(inputStr); i++) {
            inputStr[i] = tolower(inputStr[i]);
        }


        if (strcmp(inputStr, "1") == 0 || strcmp(inputStr, "manual") == 0) {
            run_round_robin_tests(MANUAL);
        } else if (strcmp(inputStr, "2") == 0 || strcmp (inputStr, "auto") == 0) {
            run_round_robin_tests(AUTOMATED);
        } else if (strcmp(inputStr, "exit") == 0) {
            break;
        } else {
            PRINT_YELLOW("Invalid input. Please enter 1, 2 or exit.\n");
        }
    }
}
