#ifndef ROUND_ROBIN_RESULT_H
#define ROUND_ROBIN_RESULT_H

#include "process.h"
#include "process_result.h"

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

  float timeQuantum;
  float totalTime;
  int contextSwitch;
} RoundRobinResult;

#endif
