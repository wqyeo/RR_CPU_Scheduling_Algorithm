#ifndef PROCESS_H
#define PROCESS_H

#define MAX_NAME_LEN 64

typedef struct {
  char name[MAX_NAME_LEN];
  float arrivalTime;
  float burstTime;
} Process;

#endif
