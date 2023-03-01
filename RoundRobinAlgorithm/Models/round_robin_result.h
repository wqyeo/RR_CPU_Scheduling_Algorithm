#ifndef ROUND_ROBIN_RESULT_H
#define ROUND_ROBIN_RESULT_H

#include "process.h"
#include "process_result.h"

typedef struct {
  char roundRobinUsed[256];

  // Use this during analysis;
  // group datasets together, marking them as the same test case.
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
