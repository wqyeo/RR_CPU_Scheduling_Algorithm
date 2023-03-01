#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

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
    ALL,
    ROUND_ROBIN,
    MANHATTEN_ROUND_ROBIN,
    BEST_QUANTUM_TIME_ROUND_ROBIN
} RoundRobinMode;

void run_application();

#endif
