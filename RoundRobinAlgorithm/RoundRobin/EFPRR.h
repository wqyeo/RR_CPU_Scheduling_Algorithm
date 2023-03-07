#ifndef EFPRR_H
#define EFPRR_H

#include "../Models/process.h"

typedef struct
{
    char processName[MAX_NAME_LEN];
    float remainingTime;
    float burstTime;
} ReadyProcess;

RoundRobinResult EFPRR(Process *processes, int processesSize, char* grouping);

#endif
