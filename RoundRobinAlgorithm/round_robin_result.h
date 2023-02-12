#ifndef ROUND_ROBIN_RESULT_H
#define ROUND_ROBIN_RESULT_H

#include "process.h"
#include "process_result.h"

typedef struct {
  ProcessResult *processResults;
  int resultSize;

  float avgWaitingTime;
  float avgTurnaroundTime;
  float avgResponseTime;
  int contextSwitch;
} RoundRobinResult;

#endif
