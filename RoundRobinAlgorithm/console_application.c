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
#include "RoundRobin/EFPRR.h"
#include "RoundRobin/sorted_round_robin.h"

#include "Util/color_print.h"
#include "Util/data_saver.h"

#define MAX_DATETIME_LEN 32
#define DEFAULT_TIME_QUANTUM 5

char *generate_random_string(int length)
{
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *random_string = malloc(sizeof(char) * (length + 1));

    for (int i = 0; i < length; i++)
    {
        int index = rand() % (int)(sizeof(charset) - 1);
        random_string[i] = charset[index];
    }

    random_string[length] = '\0';

    return random_string;
}

char* get_current_datetime_string()
{
    char* datetime = malloc(MAX_DATETIME_LEN * sizeof(char));
    if (!datetime)
    {
        return NULL; // failed to allocate memory
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // format the date-time string
    strftime(datetime, MAX_DATETIME_LEN, "%Y-%m-%d %H:%M:%S", t);

    return datetime;
}

void print_round_robin_result(RoundRobinResult result)
{
    int i;
    printf("\nCPU Scheduling Algorithm (%s)\n", result.roundRobinUsed);

    printf("\nTime Quantum Used: %d", result.timeQuantumUsed);
    printf("\nTime Quantums:[");
    for (i = 0; i < result.timeQuantumUsed; ++i)
    {
        printf("%.2f", result.timeQuantums[i]);
        // Apply comma if not ending, so it prints like [1, 3]
        if (i + 1 < result.timeQuantumUsed)
        {
            printf(", ");
        }
    }
    printf("]");

    // Print out a table of the times for each of the process.
    printf("\nPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (i = 0; i < result.processesSize; i++)
    {
        printf("%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
               result.processResults[i].process.name, result.processResults[i].process.burstTime, result.processResults[i].waitingTime, result.processResults[i].turnaroundTime, result.processResults[i].responseTime);
    }

    printf("\nAverage Waiting Time: %.2f\n", result.avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", result.avgTurnaroundTime);
    printf("Average Response Time: %.2f\n", result.avgResponseTime);
    printf("Context Switch: %d\n", result.contextSwitch);
    printf("Total Time Taken: %.2f\n", result.totalTime);
}

int request_processes_count()
{
    int processesCount = 0;
    char input[MAX_USER_INPUT_SIZE];

    while (1)
    {
        PRINT_WHITE("\n\tInput the number of processes: ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            fprintf(stderr, "Error reading input, ensure its all numbers\n");
            continue;
        }
        if (sscanf(input, "%d", &processesCount) == 1)
        {
            // input is a valid number
            break;
        }
        PRINT_YELLOW("\tInvalid input, please enter a number\n");
    }

    return processesCount;
}

void clear_input_buffer()
{
    while (getchar() != '\n');
}

Process *request_processes(int size)
{
    Process *processes = malloc(size * sizeof(Process));

    int i;
    for (i = 0; i < size; i++)
    {
        printf("\nEnter name for process %d (max %d characters): ", i, MAX_NAME_LEN-1);
        scanf("%s", processes[i].name);

        // Re-request until valid input is entered for burst time
        while (1)
        {
            printf("Enter burst time for process %s: ", processes[i].name);
            if (scanf("%f", &processes[i].burstTime) == 1)
            {
                break;
            }
            else
            {
                printf("Invalid input. Please enter a valid number.\n");
                clear_input_buffer();
            }
        }

        // Re-request until valid input is entered for arrival time
        while (1)
        {
            printf("Enter arrival time for process %s: ", processes[i].name);
            if (scanf("%f", &processes[i].arrivalTime) == 1)
            {
                break;
            }
            else
            {
                printf("Invalid input. Please enter a valid number.\n");
                clear_input_buffer();
            }
        }
    }

    return processes;
}

_RoundRobinArray request_round_robin_mode()
{
    char inputStr[MAX_USER_INPUT_SIZE];
    RoundRobinMode* modeArray = malloc(5 * sizeof(RoundRobinMode));;

    while (1)
    {
        PRINT_WHITE("\nSelect Mode:\n");
        PRINT_GRAY("1 - All (Run all implemented round robin tests) \n");
        PRINT_GRAY("2 - Round Robin\n");
        PRINT_GRAY("3 - Manhatten Round Robin\n");
        PRINT_GRAY("4 - Best Time Quantum Round Robin\n");
        PRINT_GRAY("5 - Eighty Five Percentile Round Robin\n");
        PRINT_GRAY("6 - Sorted Round Robin\n");
        PRINT_WHITE("(1-6):\n");
        fgets(inputStr, MAX_USER_INPUT_SIZE, stdin);
        // Remove the 'enter' key
        inputStr[strcspn(inputStr, "\n")] = '\0';

        for (int i = 0; i < strlen(inputStr); i++)
        {
            inputStr[i] = tolower(inputStr[i]);
        }

        if (strcmp(inputStr, "1") == 0)
        {
            // User wants all round robin modes
            modeArray[0] = ROUND_ROBIN;
            modeArray[1] = MANHATTEN_ROUND_ROBIN;
            modeArray[2] = BEST_QUANTUM_TIME_ROUND_ROBIN;
            modeArray[3] = EIGHT_FIVE_PERCENTILE_ROUND_ROBIN;
            modeArray[4] = SORTED_ROUND_ROBIN;

            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 3
            };
        }
        else if (strcmp(inputStr, "2") == 0)
        {
            modeArray[0] = ROUND_ROBIN;
            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 1
            };
        }
        else if (strcmp(inputStr, "3") == 0)
        {
            modeArray[0] = MANHATTEN_ROUND_ROBIN;
            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 1
            };
        }
        else if (strcmp(inputStr, "4") == 0)
        {
            modeArray[0] = BEST_QUANTUM_TIME_ROUND_ROBIN;
            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 1
            };
        }
        else if (strcmp(inputStr, "5") == 0)
        {
            modeArray[0] = EIGHT_FIVE_PERCENTILE_ROUND_ROBIN;
            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 1
            };
        }
        else if (strcmp(inputStr, "6") == 0)
        {
            modeArray[0] = SORTED_ROUND_ROBIN;
            return (_RoundRobinArray)
            {
                .modes = modeArray, .size = 1
            };
        }
        else
        {
            PRINT_YELLOW("Invalid input. Please enter 1-6.\n");
        }
    }
    // Shouldn't reach here.
    return (_RoundRobinArray)
    {
        .modes = modeArray, .size = 0
    };
}

RoundRobinResult perform_round_robin(Process *processes, int processesCount, char* groupingString, RunMode runMode)
{
    float timeQuantum;
    // Re-request until valid input is entered for burst time
    while (1 && runMode != GENERATOR)
    {
        printf("Enter Quantum Time for Round Robin ");
        if (scanf("%f", &timeQuantum) == 1)
        {
            break;
        }
        else
        {
            printf("Invalid input. Please enter a valid number.\n");
            clear_input_buffer();
        }
    }

    if (runMode == GENERATOR)
    {
        timeQuantum = DEFAULT_TIME_QUANTUM;
    }

    return round_robin(processes, processesCount, timeQuantum, groupingString);
}

// Deep clone
Process* clone_processes(Process* processes, int size)
{
    Process* new_processes = malloc(size * sizeof(Process));
    int i;
    for (i = 0; i < size; i++)
    {
        // copy the name of the process
        strcpy(new_processes[i].name, processes[i].name);

        // copy the burst and arrival times
        new_processes[i].burstTime = processes[i].burstTime;
        new_processes[i].arrivalTime = processes[i].arrivalTime;
    }
    return new_processes;
}

void run_round_robin_tests(RunMode runMode, int processCount, char* fileNameExtender)
{
    Process* processes;
    // Either manually get processes from user's input
    // or generate processes.
    if (runMode == MANUAL)
    {
        processes = request_processes(processCount);
    }
    else
    {
        processes = generate_random_processes(processCount);
    }

    // Either run all round robin types,
    // or user manually inputs which round robin to use.
    _RoundRobinArray roundRobinsToUse;
    if (runMode == GENERATOR)
    {
        RoundRobinMode* roundRobinModes = malloc(6 * sizeof(RoundRobinMode));
        roundRobinModes[0] = ROUND_ROBIN;
        roundRobinModes[1] = MANHATTEN_ROUND_ROBIN;
        roundRobinModes[2] = BEST_QUANTUM_TIME_ROUND_ROBIN;
        roundRobinModes[3] = SORTED_ROUND_ROBIN;
        roundRobinModes[4] = EIGHT_FIVE_PERCENTILE_ROUND_ROBIN;

        roundRobinsToUse.modes = roundRobinModes;
        roundRobinsToUse.size = 5;
    }
    else
    {
        roundRobinsToUse = request_round_robin_mode();
    }

    char fileName[54] = "";
    char* groupingString = generate_random_string(62);

    for (size_t i = 0; i < roundRobinsToUse.size; ++i)
    {
        // Clone the process, and used the cloned process,
        // to prevent modifying the original.
        // (Original needed if we are to perform more round robin simulations)
        Process* clonedProcesses = clone_processes(processes, processCount);
        RoundRobinResult roundRobinResult;
        int simulatedFlag = 1;
        if (roundRobinsToUse.modes[i] == ROUND_ROBIN)
        {
            strcpy(fileName,"RoundRobin_");
            roundRobinResult = perform_round_robin(clonedProcesses, processCount, groupingString, runMode);
        }
        else if (roundRobinsToUse.modes[i] == MANHATTEN_ROUND_ROBIN)
        {
            strcpy(fileName,"Manhatten_");
            roundRobinResult = manhattan_round_robin(clonedProcesses, processCount, groupingString);
        }
        else if (roundRobinsToUse.modes[i] == BEST_QUANTUM_TIME_ROUND_ROBIN)
        {
            strcpy(fileName,"BestTimeQuantum_");
            roundRobinResult = modified_round_robin(clonedProcesses, processCount, groupingString);
        }
        else if (roundRobinsToUse.modes[i] == SORTED_ROUND_ROBIN)
        {
            strcpy(fileName,"SortedRoundRobin_");
            roundRobinResult = sorted_round_robin(clonedProcesses, processCount, groupingString);
        }
        else if (roundRobinsToUse.modes[i] == EIGHT_FIVE_PERCENTILE_ROUND_ROBIN)
        {
            strcpy(fileName,"EFPRR_");
            roundRobinResult = EFPRR(clonedProcesses, processCount, groupingString);
        }
        else
        {
            PRINT_RED("Unimplemented Round Robin mode called");
            simulatedFlag = 0;
            continue;
        }

        if (simulatedFlag == 1)
        {
            print_round_robin_result(roundRobinResult);
            strcat(fileName, get_current_datetime_string());
            strcat(fileName, fileNameExtender);
            save_result_to_file(fileName, roundRobinResult);
            free(roundRobinResult.processResults);
            //free(roundRobinResult.timeQuantums);
        }

        free(clonedProcesses);
        PRINT_BLUE("\n=================\n");
    }

    free(roundRobinsToUse.modes);
    free(groupingString);
    free(processes);
}


char* int_to_string(int num)
{
    // Allocate memory for string
    char* str = malloc((sizeof(char) * 8) + 1);
    if (!str)
    {
        return 0;
    }

    sprintf(str, "%d", num);
    return str;
}

void repeat_do_test(int repeatCount)
{
    char fileNameExtender[32] = "_";
    int i = 0;
    while (i < repeatCount)
    {
        int processesCount = (rand() % 20) + 20;

        strcpy(fileNameExtender, "_");
        char* iStr = int_to_string(i);
        strcat(fileNameExtender, iStr);

        run_round_robin_tests(GENERATOR, processesCount, fileNameExtender);
        ++i;

        free(iStr);
    }
}

void run_application()
{
    srand(time(NULL));
    PRINT_WHITE("Round Robin Simulator...\n");
    char inputStr[MAX_USER_INPUT_SIZE];

    while (1)
    {
        PRINT_WHITE("\nSelect Mode:\n");
        PRINT_GRAY("1 (Manual) - Manually input processes.\n");
        PRINT_GRAY("2 (Auto) - Automatically generate processes, input the niches of the Round Robin.\n");
        PRINT_GRAY("3 (Generate) - Runs everything automatically by randomly generating all the inputs.\n");
        PRINT_WHITE("(1/2/3/EXIT):\n");
        fgets(inputStr, MAX_USER_INPUT_SIZE, stdin);

        // Remove the 'enter' key
        inputStr[strcspn(inputStr, "\n")] = '\0';

        for (int i = 0; i < strlen(inputStr); i++)
        {
            inputStr[i] = tolower(inputStr[i]);
        }

        if (strcmp(inputStr, "1") == 0 || strcmp(inputStr, "manual") == 0)
        {
            int processCount = request_processes_count();
            run_round_robin_tests(MANUAL, processCount, "");
        }
        else if (strcmp(inputStr, "2") == 0 || strcmp (inputStr, "auto") == 0)
        {
            int processCount = request_processes_count();
            run_round_robin_tests(AUTOMATED, processCount, "");
        }
        else if (strcmp(inputStr, "3") == 0 || strcmp(inputStr, "generate") == 0)
        {
            // NOTE: Probably can generate this test case count as well.
            repeat_do_test(30);
        }
        else if (strcmp(inputStr, "exit") == 0)
        {
            break;
        }
        else
        {
            PRINT_YELLOW("Invalid input. Please enter 1, 2, 3 or exit.\n");
        }
    }
}
