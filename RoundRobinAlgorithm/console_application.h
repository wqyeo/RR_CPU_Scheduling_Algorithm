#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "stddef.h"

#define MAX_USER_INPUT_SIZE 64

typedef enum {
  YES,
  NO,
  CANCEL
} UserPrompt;

typedef enum {
  MANUAL,
  AUTOMATED,
  GENERATOR
} RunMode;

typedef enum {
    ROUND_ROBIN,
    MANHATTEN_ROUND_ROBIN,
    BEST_QUANTUM_TIME_ROUND_ROBIN
} RoundRobinMode;

typedef struct {
  RoundRobinMode* modes;
  size_t size;
} _RoundRobinArray;

void run_application();

#endif
