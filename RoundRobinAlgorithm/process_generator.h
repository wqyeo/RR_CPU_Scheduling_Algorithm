#ifndef PROCESS_GENERATOR_H
#define PROCESS_GENERATOR_H

#define MAX_BURST_TIME 40
#define MIN_BURST_TIME 1
#define MAX_ARRIVAL_TIME 20

#include "Models/process.h"

Process* generate_random_processes(int processesSize);
#endif
