#ifndef PROCESS_RESULT_H
#define PROCESS_RESULT_H

#include "process.h"

typedef struct {
  Process process;

  float waitingTime;
  float turnaroundTime;
  float responseTime;
} ProcessResult;

#endif
