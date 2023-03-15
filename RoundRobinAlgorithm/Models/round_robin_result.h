#ifndef ROUND_ROBIN_RESULT_H
#define ROUND_ROBIN_RESULT_H

#include "process.h"
#include "process_result.h"

#define MAX_TIME_QUANTUM_STORE_COUNT 512

typedef struct {
  char roundRobinUsed[256];

  // Use this during analysis;
  // group datasets together, marking them as having received the same test case inputs.
  char grouping[64];

  ProcessResult *processResults;
  int processesSize;

  float avgWaitingTime;
  float avgTurnaroundTime;
  float avgResponseTime;

  // Because some algorithm can use more than one time quantum
  // (The time quantum can change during the execution of algorithm)
  int timeQuantumUsed;
  float timeQuantums[MAX_TIME_QUANTUM_STORE_COUNT];

  float totalTime;
  int contextSwitch;
} RoundRobinResult;

#endif
