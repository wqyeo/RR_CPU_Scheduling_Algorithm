#ifndef READY_PROCESS_H
#define READY_PROCESS_H

#include "../Models/process.h"

/**
* Used to represent a process that is ready to be executed.
* For the ready queue.
*/
typedef struct
{
    char processName[MAX_NAME_LEN];
    float remainingTime;
    float burstTime;
} ReadyProcess;

#endif
