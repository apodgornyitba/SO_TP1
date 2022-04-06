#ifndef APP_H
#define APP_H

#include "library.h"

#define SLAVE_AMOUNT 8
#define SLAVE_PATH "./Slave"

typedef struct {
    pid_t pid;
    int in;
    int out;
} slave;

void createSlave(slave slavesArray[], int slaveAmount, char *path, char *const argv[]);
void killSlave(slave slavesArray[], int slaveAmount);

#endif